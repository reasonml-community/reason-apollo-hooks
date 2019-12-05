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
  (
    ~variables: Js.Json.t=?,
    ~client: ApolloClient.generatedApolloClient=?,
    ApolloHooksTypes.graphqlDefinition('data, _, _)
  ) =>
  (variant('data), result('data)) =
  (~variables=?, ~client=?, (parse, query, _)) => {
    let jsResult =
      useSubscriptionJs(gql(. query), options(~variables?, ~client?, ()));

    let result = {
      data: jsResult##data->Js.Nullable.toOption->Belt.Option.map(parse),
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
