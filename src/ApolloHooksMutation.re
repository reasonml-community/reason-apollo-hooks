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
    jsExecutionResult('raw_t) => array(ApolloClient.queryObj);

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

type optimisticResult;

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
type options('raw_t) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  mutation: option(ReasonApolloTypes.queryString),
  [@bs.optional]
  client: ApolloClient.generatedApolloClient('raw_t),
  [@bs.optional]
  refetchQueries: Execution.refetchQueries('raw_t),
  [@bs.optional]
  awaitRefetchQueries: bool,
  [@bs.optional]
  update:
    (ApolloClient.generatedApolloClient('raw_t), mutationResult('raw_t)) =>
    unit,
  [@bs.optional]
  optimisticResponse: optimisticResult,
};

type jsMutate('raw_t) =
  (. options('raw_t)) => Js.Promise.t(Execution.jsExecutionResult('raw_t));

type mutation('t, 'raw_t) =
  (
    ~variables: Js.Json.t=?,
    ~client: ApolloClient.generatedApolloClient('raw_t)=?,
    ~refetchQueries: Execution.refetchQueries('raw_t)=?,
    ~awaitRefetchQueries: bool=?,
    ~optimisticResponse: optimisticResult=?,
    unit
  ) =>
  Js.Promise.t(
    (Execution.executionVariantResult('t), Execution.executionResult('t)),
  );

[@bs.module "@apollo/client"]
external useMutationJs:
  (. ReasonApolloTypes.queryString, options('raw_t)) =>
  (jsMutate('raw_t), jsResult('raw_t)) =
  "useMutation";

exception Error(string);

let useMutation:
  (
    ~client: ApolloClient.generatedApolloClient('raw_t)=?,
    ~variables: Js.Json.t=?,
    ~refetchQueries: Execution.refetchQueries('raw_t)=?,
    ~awaitRefetchQueries: bool=?,
    ~update: (
               ApolloClient.generatedApolloClient('raw_t),
               mutationResult('raw_t)
             ) =>
             unit
               =?,
    ~optimisticResponse: optimisticResult=?,
    ApolloHooksTypes.graphqlDefinition('t, 'raw_t, _)
  ) =>
  (mutation('t, 'raw_t), controlledVariantResult('t), controlledResult('t)) =
  (
    ~client=?,
    ~variables=?,
    ~refetchQueries=?,
    ~awaitRefetchQueries=?,
    ~update=?,
    ~optimisticResponse=?,
    (parse, query, _),
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
          ~optimisticResponse?,
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
              ~optimisticResponse?,
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
