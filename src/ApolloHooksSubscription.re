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
external useSubscriptionJs:
  (ReasonApolloTypes.queryString, options) =>
  {
    .
    "data": Js.Nullable.t(Js.Json.t),
    "loading": bool,
    "error": Js.Nullable.t(error),
  } =
  "useSubscription";

let useSubscription:
  type t.
    (
      ~variables: Js.Json.t=?,
      ~client: ApolloClient.generatedApolloClient=?,
      (module ApolloHooksTypes.Config with type t = t)
    ) =>
    (variant(t), result(t)) =
  (~variables=?, ~client=?, (module Config)) => {
    let jsResult =
      useSubscriptionJs(
        gql(. Config.query),
        options(~variables?, ~client?, ()),
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
