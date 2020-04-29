open ApolloHooksTypes;

type jsResult('raw_t) = {
  .
  "data": Js.Nullable.t('raw_t),
  "loading": bool,
  "called": bool,
  "error": Js.Nullable.t(apolloError),
};

module Execution = {
  type jsExecutionResult('raw_t) = {
    .
    "data": Js.Nullable.t('raw_t),
    "errors": Js.Nullable.t(array(graphqlError)),
  };

  type refetchQueries('raw_t) =
    jsExecutionResult('raw_t) => array(ApolloClient.opaqueQueryObj);

  /* The type that the promise returned by the mutate function resolves to */
  type executionResult('a) = {
    data: option('a),
    errors: option(array(graphqlError)),
  };

  type executionVariantResult('a) =
    | Data('a)
    | Errors(array(graphqlError))
    | NoData;
};

/* The type of the 'full' result returned by the hook */
type controlledResult('t) = {
  loading: bool,
  called: bool,
  data: option('t),
  error: option(apolloError),
};

/* The type of the 'simple' result returned by the hook */
type controlledVariantResult('t) =
  | Loading
  | NotCalled
  | Data('t)
  | Error(apolloError)
  | NoData;

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

type mutationResult('raw_t) = {. "data": option('raw_t)};

[@bs.deriving abstract]
type options('raw_t, 'raw_t_variables) = {
  [@bs.optional]
  variables: 'raw_t_variables,
  [@bs.optional]
  mutation: option(ReasonApolloTypes.queryString),
  [@bs.optional]
  client: ApolloClient.t,
  [@bs.optional]
  refetchQueries: Execution.refetchQueries('raw_t),
  [@bs.optional]
  awaitRefetchQueries: bool,
  [@bs.optional]
  update: (ApolloClient.t, mutationResult('raw_t)) => unit,
  [@bs.optional]
  optimisticResponse: 'raw_t,
};

type jsMutate('raw_t, 'raw_t_variables) =
  (. options('raw_t, 'raw_t_variables)) =>
  Js.Promise.t(Execution.jsExecutionResult('raw_t));

type mutation('t, 'raw_t, 'raw_t_variables) =
  (
    ~variables: 'raw_t_variables=?,
    ~client: ApolloClient.t=?,
    ~refetchQueries: Execution.refetchQueries('raw_t)=?,
    ~awaitRefetchQueries: bool=?,
    ~optimisticResponse: 't=?,
    unit
  ) =>
  Js.Promise.t(
    (Execution.executionVariantResult('t), Execution.executionResult('t)),
  );

[@bs.module "@apollo/client"]
external useMutationJs:
  (. ReasonApolloTypes.queryString, options('raw_t, 'raw_t_variables)) =>
  (jsMutate('raw_t, 'raw_t_variables), jsResult('raw_t)) =
  "useMutation";

exception Error(string);

let useMutation:
  (
    ~client: ApolloClient.t=?,
    ~variables: 'raw_t_variables=?,
    ~refetchQueries: Execution.refetchQueries('raw_t)=?,
    ~awaitRefetchQueries: bool=?,
    ~update: (ApolloClient.t, mutationResult('raw_t)) => unit=?,
    ~optimisticResponse: 't=?,
    ApolloHooksTypes.graphqlDefinition('t, 'raw_t)
  ) =>
  (
    mutation('t, 'raw_t, 'raw_t_variables),
    controlledVariantResult('t),
    controlledResult('t),
  ) =
  (
    ~client=?,
    ~variables=?,
    ~refetchQueries=?,
    ~awaitRefetchQueries=?,
    ~update=?,
    ~optimisticResponse=?,
    (parse, query, serialize),
  ) => {
    let (jsMutate, jsResult) =
      useMutationJs(.
        gql(. query),
        options(
          ~client?,
          ~variables?,
          ~refetchQueries?,
          ~awaitRefetchQueries?,
          ~update?,
          ~optimisticResponse=?
            switch (optimisticResponse) {
            | Some(optimisticResponse) =>
              Some(serialize(optimisticResponse))
            | None => None
            },
          (),
        ),
      );

    let mutate =
      React.useMemo1(
        (
          (),
          ~variables=?,
          ~client=?,
          ~refetchQueries=?,
          ~awaitRefetchQueries=?,
          ~optimisticResponse=?,
          (),
        ) =>
          jsMutate(.
            options(
              ~variables?,
              ~client?,
              ~refetchQueries?,
              ~awaitRefetchQueries?,
              ~optimisticResponse=?
                switch (optimisticResponse) {
                | Some(optimisticResponse) =>
                  Some(serialize(optimisticResponse))
                | None => None
                },
              (),
            ),
          )
          |> Js.Promise.then_(jsResult => {
               let full =
                 Execution.{
                   data:
                     Js.Nullable.toOption(jsResult##data)
                     ->Belt.Option.map(parse),
                   errors:
                     switch (Js.Nullable.toOption(jsResult##errors)) {
                     | Some(errors) when Js.Array.length(errors) > 0 =>
                       Some(errors)
                     | _ => None
                     },
                 };

               let simple =
                 switch (full) {
                 | {errors: Some(errors)} => (
                     Errors(errors): Execution.executionVariantResult('data)
                   )
                 | {data: Some(data)} => Data(data)
                 | {errors: None, data: None} => NoData
                 };

               (simple, full) |> Js.Promise.resolve;
             }),
        [|variables|],
      );

    let full =
      React.useMemo1(
        () =>
          {
            loading: jsResult##loading,
            called: jsResult##called,
            data:
              jsResult##data->Js.Nullable.toOption->Belt.Option.map(parse),
            error: jsResult##error->Js.Nullable.toOption,
          },
        [|jsResult|],
      );

    let simple =
      React.useMemo1(
        () =>
          switch (full) {
          | {loading: true} => Loading
          | {error: Some(error)} => Error(error)
          | {data: Some(data)} => Data(data)
          | {called: false} => NotCalled
          | _ => NoData
          },
        [|full|],
      );

    (mutate, simple, full);
  };
