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

type onSubscriptionDataOptions('a) = {
  client: ApolloClient.generatedApolloClient,
  subscriptionData: 'a,
};

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

[@bs.deriving abstract]
type options('a) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  skip: bool,
  [@bs.optional]
  onSubscriptionData: 'a => unit,
  [@bs.optional]
  client: ApolloClient.generatedApolloClient,
};

[@bs.module "@apollo/react-hooks"]
external useSubscriptionJs:
  (ReasonApolloTypes.queryString, options('a)) =>
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
    ~skip: bool=?,
    ~onSubscriptionData: onSubscriptionDataOptions('data) => unit=?,
    ApolloHooksTypes.graphqlDefinition('data, _, _)
  ) =>
  (variant('data), result('data)) =
  (
    ~variables=?,
    ~client=?,
    ~skip=?,
    ~onSubscriptionData=?,
    (parse, query, _),
  ) => {
    let jsOnSubscriptionData =
      Belt.Option.map(onSubscriptionData, (onSubscriptionDataFn, a) =>
        onSubscriptionDataFn({
          client: a##client,
          subscriptionData: a##subscriptionData##data->parse,
        })
      );

    let jsResult =
      useSubscriptionJs(
        gql(. query),
        options(
          ~variables?,
          ~client?,
          ~skip?,
          ~onSubscriptionData=?jsOnSubscriptionData,
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
