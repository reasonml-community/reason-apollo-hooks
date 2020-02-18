open ApolloHooksTypes;

type jsResult = {
  .
  "data": Js.Nullable.t(Js.Json.t),
  "loading": bool,
  "called": bool,
  "error": Js.Nullable.t(apolloError),
};

type jsExecutionResult = {
  .
  "data": Js.Nullable.t(Js.Json.t),
  "errors": Js.Nullable.t(array(graphqlError)),
};

type refetchQueries = jsExecutionResult => array(ApolloClient.queryObj);

/* The type that the promise returned by the mutate function resolves to */
type executionResult('a) = {
  data: option('a),
  errors: option(array(graphqlError)),
};

type executionVariantResult('a) =
  | Data('a)
  | Errors(array(graphqlError))
  | NoData;

/* The type of the 'full' result returned by the hook */
type controlledResult('a) = {
  loading: bool,
  called: bool,
  data: option('a),
  error: option(apolloError),
};

/* The type of the 'simple' result returned by the hook */
type controlledVariantResult('a) =
  | Loading
  | NotCalled
  | Data('a)
  | Error(apolloError)
  | NoData;

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

type mutationResult('a) = {. "data": option('a)};

[@bs.deriving abstract]
type options('a) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  mutation: option(ReasonApolloTypes.queryString),
  [@bs.optional]
  client: ApolloClient.generatedApolloClient,
  [@bs.optional]
  refetchQueries,
  [@bs.optional]
  awaitRefetchQueries: bool,
  [@bs.optional]
  update: (ApolloClient.generatedApolloClient, mutationResult('a)) => unit,
  [@bs.optional]
  optimisticResponse: Js.Json.t,
};

type jsMutate('a) = (. options('a)) => Js.Promise.t(jsExecutionResult);

type mutation('a) =
  (
    ~variables: Js.Json.t=?,
    ~client: ApolloClient.generatedApolloClient=?,
    ~refetchQueries: refetchQueries=?,
    ~awaitRefetchQueries: bool=?,
    ~optimisticResponse: Js.Json.t=?,
    unit
  ) =>
  Js.Promise.t((executionVariantResult('a), executionResult('a)));

[@bs.module "@apollo/react-hooks"]
external useMutationJs:
  (. ReasonApolloTypes.queryString, options('a)) => (jsMutate('a), jsResult) =
  "useMutation";

exception Error(string);

let useMutation:
  (
    ~client: ApolloClient.generatedApolloClient=?,
    ~variables: Js.Json.t=?,
    ~refetchQueries: refetchQueries=?,
    ~awaitRefetchQueries: bool=?,
    ~update: (ApolloClient.generatedApolloClient, mutationResult('data)) =>
             unit
               =?,
    ~optimisticResponse: Js.Json.t=?,
    ApolloHooksTypes.graphqlDefinition('data, _, _)
  ) =>
  (
    mutation('data),
    controlledVariantResult('data),
    controlledResult('data),
  ) =
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
               let full = {
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
                     Errors(errors): executionVariantResult('data)
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
