module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

type error = {. "message": string};

type result('a) =
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

  [@bs.module "@apollo/react-hooks"]
  external useMutation:
    (. ReasonApolloTypes.queryString, (options)) =>
    (. options) =>
    Js.Promise.t({
      .
      "data": Js.Nullable.t(Js.Json.t),
      "error": Js.Nullable.t(error),
    }) =
    "useMutation";

  let use = (~variables=?, ~client=?, ()) => {
    let jsMutate =
      useMutation(. gql(. Config.query), (options(~variables?, ~client?, ())));

    let mutate = (~variables=?, ~client=?, ()) =>
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
         );

    mutate
  };
};
