type graphqlErrors;

type error = {
  .
  "message": string,
  "graphqlErrors": graphqlErrors,
};

type refetchQueries =
  ReasonApolloTypes.executionResult => array(ApolloClient.queryObj);

/* Result that is return by the hook */
type result('a) =
  | Data('a)
  | Error(error)
  | NoData;

/* Result that is return by the hook */
type controlledResult('a) = {
  loading: bool,
  called: bool,
  data: option('a),
  error: option(error),
};

type controlledVariantResult('a) =
  | Loading
  | Called
  | Data('a)
  | Error(error)
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
};

type jsResult = {
  .
  "data": Js.Nullable.t(Js.Json.t),
  "loading": bool,
  "called": bool,
  "error": Js.Nullable.t(error),
};

type jsMutate('a) = (. options('a)) => Js.Promise.t(jsResult);
type mutation('a) =
  (
    ~variables: Js.Json.t=?,
    ~client: ApolloClient.generatedApolloClient=?,
    ~refetchQueries: refetchQueries=?,
    ~awaitRefetchQueries: bool=?,
    unit
  ) =>
  Js.Promise.t(controlledVariantResult('a));

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
          (),
        ) =>
          jsMutate(.
            options(
              ~variables?,
              ~client?,
              ~refetchQueries?,
              ~awaitRefetchQueries?,
              (),
            ),
          )
          |> Js.Promise.then_(jsResult =>
               (
                 switch (
                   Js.Nullable.toOption(jsResult##data),
                   Js.Nullable.toOption(jsResult##error),
                 ) {
                 | (Some(data), _) => Data(parse(data))
                 | (None, Some(error)) => Error(error)
                 | (None, None) => NoData
                 }
               )
               |> Js.Promise.resolve
             ),
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
          | {called: true} => Called
          | _ => NoData
          },
        [|full|],
      );

    (mutate, simple, full);
  };
