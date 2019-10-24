module type Config = {
  let query: string;
  type t;
  let parse: Js.Json.t => t;
};

type error = {. "message": string};

type variant('a) =
  | Data('a)
  | Error(error)
  | Loading
  | NoData;

type result('a) = {
  data: option('a),
  loading: bool,
  error: option(error),
};

module Make = (Config: Config) => {
  [@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

  [@bs.deriving abstract]
  type options = {
    [@bs.optional]
    variables: Js.Json.t,
    [@bs.optional]
    skip: bool,
    [@bs.optional]
    onSubscriptionData: unit => unit,
    [@bs.optional]
    client: ApolloClient.generatedApolloClient,
  };

  [@bs.module "@apollo/react-hooks"]
  external useSubscription:
    (ReasonApolloTypes.queryString, (options)) =>
    {
      .
      "data": Js.Nullable.t(Js.Json.t),
      "loading": bool,
      "error": Js.Nullable.t(error),
    } =
    "useSubscription";

  let use = (~variables=?, ~client=?, ()) => {
    let jsResult =
      useSubscription(gql(. Config.query), options(~variables?, ~client?, ()));

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
