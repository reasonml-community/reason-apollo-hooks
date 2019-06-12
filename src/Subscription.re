module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

module Make = (Config: Config) => {
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

  type error = {. "message": string};

  [@bs.deriving abstract]
  type options = {
    [@bs.optional]
    skip: bool,
    [@bs.optional]
    onSubscriptionData: unit => unit,
    [@bs.optional]
    client: ApolloClient.generatedApolloClient,
  };

  [@bs.module "react-apollo-hooks"]
  external useSubscription:
    (ReasonApolloTypes.queryString, Js.Nullable.t( options )) =>
    {
      .
      "data": Js.Nullable.t(Js.Json.t),
      "loading": bool,
      "error": Js.Nullable.t(error),
    } =
    "useSubscription";

  type variant =
    | Data(Config.t)
    | Error(error)
    | Loading
    | NoData;
  type result = {
    data: option(Config.t),
    loading: bool,
    error: option(error),
  };

  let use = (~options=?, ()) => {
    let jsResult =
      useSubscription(
        gql(. Config.query ),
        Js.Nullable.fromOption( options ),
      );

    let result = {
      data:
        jsResult##data->Js.Nullable.toOption->Belt.Option.map(Config.parse),
      loading: jsResult##loading,
      error: jsResult##error->Js.Nullable.toOption,
    };

    (
      switch (result) {
      | {data: Some(data)} => Data(data)
      | {error: Some(error)} => Error(error)
      | {loading: true} => Loading
      | _ => NoData
      },
      result,
    );
  };
};
