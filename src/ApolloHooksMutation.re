type graphqlErrors;

type error = {
  .
  "message": string,
  "graphqlErrors": graphqlErrors,
};

type refetchQueries('raw_t) =
  ReasonApolloTypes.executionResult('raw_t) => array(ApolloClient.queryObj);

/* Result that is return by the hook */
type result('a) =
  | Data('a)
  | Error(error)
  | NoData;

/* Result that is return by the hook */
type controlledResult('t) = {
  loading: bool,
  called: bool,
  data: option('t),
  error: option(error),
};

type optimisticResult;

type controlledVariantResult('t) =
  | Loading
  | Called
  | Data('t)
  | Error(error)
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
  refetchQueries: refetchQueries('raw_t),
  [@bs.optional]
  awaitRefetchQueries: bool,
  [@bs.optional]
  update:
    (ApolloClient.generatedApolloClient('raw_t), mutationResult('raw_t)) =>
    unit,
  [@bs.optional]
  optimisticResponse: optimisticResult,
};

type jsResult('raw_t) = {
  .
  "data": Js.Nullable.t('raw_t),
  "loading": bool,
  "called": bool,
  "error": Js.Nullable.t(error),
};

type jsMutate('raw_t) =
  (. options('raw_t)) => Js.Promise.t(jsResult('raw_t));
type mutation('t, 'raw_t) =
  (
    ~variables: Js.Json.t=?,
    ~client: ApolloClient.generatedApolloClient('raw_t)=?,
    ~refetchQueries: refetchQueries('raw_t)=?,
    ~awaitRefetchQueries: bool=?,
    ~optimisticResponse: optimisticResult=?,
    unit
  ) =>
  Js.Promise.t(controlledVariantResult('t));

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
    ~refetchQueries: refetchQueries('raw_t)=?,
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
