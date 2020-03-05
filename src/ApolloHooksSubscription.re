open ApolloHooksTypes;

type variant('a) =
  | Data('a)
  | Error(apolloError)
  | Loading
  | NoData;

type result('a) = {
  data: option('a),
  loading: bool,
  error: option(apolloError),
};

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

type onSubscriptionDataJsT('a) = {
  .
  "subscriptionData": {. "data": 'a},
  "client": ApolloClient.generatedApolloClient,
};

type onSubscriptionDataT('a) = {
  subscriptionData: 'a,
  client: ApolloClient.generatedApolloClient,
};

[@bs.deriving abstract]
type options('data) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  skip: bool,
  [@bs.optional]
  onSubscriptionData: onSubscriptionDataJsT('data) => unit,
  [@bs.optional]
  client: ApolloClient.generatedApolloClient,
};

[@bs.module "@apollo/react-hooks"]
external useSubscriptionJs:
  (ReasonApolloTypes.queryString, options('data)) =>
  {
    .
    "data": Js.Nullable.t(Js.Json.t),
    "loading": bool,
    "error": Js.Nullable.t(apolloError),
  } =
  "useSubscription";

let useSubscription:
  (
    ~variables: Js.Json.t=?,
    ~client: ApolloClient.generatedApolloClient=?,
    ~onSubscriptionData: onSubscriptionDataT('data) => unit=?,
    ApolloHooksTypes.graphqlDefinition('data, _, _)
  ) =>
  (variant('data), result('data)) =
  (~variables=?, ~client=?, ~onSubscriptionData=?, (parse, query, _)) => {
    let jsResult =
      useSubscriptionJs(
        gql(. query),
        options(
          ~variables?,
          ~client?,
          ~onSubscriptionData=?
            onSubscriptionData->Belt.Option.map((f, x) =>
              f({
                client: x##client,
                subscriptionData: x##subscriptionData##data,
              })
            ),
          (),
        ),
      );

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
