module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

module Make = (Config: Config) => {
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

  [@bs.deriving abstract]
  type options = {
    [@bs.optional]
    variables: Js.Json.t,
    [@bs.optional]
    client: ApolloClient.generatedApolloClient,
  };

  type error = {. "message": string};

  [@bs.module "react-apollo-hooks"]
  external useMutation:
    (
      .
      ReasonApolloTypes.queryString,
      Js.Nullable.t(options),
    ) => (
      .
      options
    ) =>
    Js.Promise.t({
      .
      "data": Js.Nullable.t(Js.Json.t),
      "error": Js.Nullable.t(error),
    }) =
    "useMutation";

  type result =
    | Data(Config.t)
    | Error(error)
    | NoData;

  let use = (~options=?, ()) => {
    let jsMutate =
      useMutation(
        .
        gql(. Config.query ),
        Js.Nullable.fromOption( options ),
      );

    options =>
      jsMutate(. options)
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
  };
};
