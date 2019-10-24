module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

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

module Make = (Config: Config) => {
  [@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

  type mutationResult = {. "data": option(Config.t)};

  [@bs.deriving abstract]
  type options = {
    [@bs.optional]
    variables: Js.Json.t,
    [@bs.optional]
    client: ApolloClient.generatedApolloClient,
    [@bs.optional]
    refetchQueries,
    [@bs.optional]
    awaitRefetchQueries: bool,
    [@bs.optional]
    update: (ApolloClient.generatedApolloClient, mutationResult) => unit,
  };

  type jsResult = {
    .
    "data": Js.Nullable.t(Js.Json.t),
    "loading": bool,
    "called": bool,
    "error": Js.Nullable.t(error),
  };

  type jsMutate = (. options) => Js.Promise.t(jsResult);
  type mutation =
    (
      ~variables: Js.Json.t=?,
      ~client: ApolloClient.generatedApolloClient=?,
      ~refetchQueries: refetchQueries=?,
      ~awaitRefetchQueries: bool=?,
      unit
    ) =>
    Js.Promise.t(controlledVariantResult(Config.t));

  [@bs.module "@apollo/react-hooks"]
  external useMutation:
    (. ReasonApolloTypes.queryString, options) => (jsMutate, jsResult) =
    "useMutation";

  let use =
      (
        ~variables=?,
        ~client=?,
        ~refetchQueries=?,
        ~awaitRefetchQueries=?,
        ~update=?,
        (),
      ) => {
    let (jsMutate, jsResult) =
      useMutation(.
        gql(. Config.query),
        options(
          ~variables?,
          ~client?,
          ~refetchQueries?,
          ~awaitRefetchQueries?,
          ~update?,
          (),
        ),
      );

    let mutate: mutation =
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
                 | (Some(data), _) => Data(Config.parse(data))
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
              jsResult##data
              ->Js.Nullable.toOption
              ->Belt.Option.map(Config.parse),
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
};
