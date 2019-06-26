module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

type graphqlErrors;

type error = {
  .
  "message": string,
  "graphlErrors": graphqlErrors,
};

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

type controledVariantResult('a) =
  | Loading
  | Called
  | Data('a)
  | Error(error)
  | NoData;

module Make = (Config: Config) => {
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

  [@bs.deriving abstract]
  type options = {
    [@bs.optional]
    variables: Js.Json.t,
    [@bs.optional]
    client: ApolloClient.generatedApolloClient,
  };

  type jsResult = {
    .
    "data": Js.Nullable.t(Js.Json.t),
    "loading": bool,
    "called": bool,
    "error": Js.Nullable.t(error),
  };

  type jsMutate = (. options) => Js.Promise.t(jsResult);

  [@bs.module "@apollo/react-hooks"]
  external useMutation:
    (. ReasonApolloTypes.queryString, options) => (jsMutate, jsResult) =
    "useMutation";

  let use = (~variables=?, ~client=?, ()) => {
    let (jsMutate, jsResult) =
      useMutation(.
        gql(. Config.query),
        options(~variables?, ~client?, ()),
      );

    let mutate =
      React.useMemo1(
        ((), ~variables=?, ~client=?, ()) =>
          jsMutate(. options(~variables?, ~client?, ()))
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

    let full = {
      loading: jsResult##loading,
      called: jsResult##called,
      data:
        jsResult##data->Js.Nullable.toOption->Belt.Option.map(Config.parse),
      error: jsResult##error->Js.Nullable.toOption,
    };

    let simple =
      switch (full) {
      | {loading: true} => Loading
      | {error: Some(error)} => Error(error)
      | {data: Some(data)} => Data(data)
      | {called: true} => Called
      | _ => NoData
      };

    (mutate, simple, full);
  };
};
