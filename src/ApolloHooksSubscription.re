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

type onSubscriptionDataJsT = {
  .
  "subscriptionData": {. "data": Js.Json.t},
  "client": ApolloClient.generatedApolloClient,
};

type onSubscriptionDataResult('data) = {
  unparsedData: Js.Json.t,
  data: 'data,
};

type onSubscriptionDataT('data) = {
  subscriptionData: onSubscriptionDataResult('data),
  client: ApolloClient.generatedApolloClient,
};

[@bs.deriving abstract]
type options('data) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  skip: bool,
  [@bs.optional]
  onSubscriptionData: onSubscriptionDataJsT => unit,
  [@bs.optional]
  client: ApolloClient.generatedApolloClient,
  [@bs.optional]
  shouldResubscribe: bool,
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
    ~shouldResubscribe: bool=?,
    ApolloHooksTypes.graphqlDefinition('data, _, _)
  ) =>
  (variant('data), result('data)) =
  (
    ~variables=?,
    ~client=?,
    ~onSubscriptionData=?,
    ~shouldResubscribe=?,
    (parse, query, _),
  ) => {
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
                subscriptionData: {
                  unparsedData: x##subscriptionData##data,
                  data: parse(x##subscriptionData##data),
                },
              })
            ),
          ~shouldResubscribe?,
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
