open ApolloHooksTypes;

type variant('a) =
  | Data('a)
  | Error(apolloError)
  | Loading
  | NoData;

/**
 *
 *  apollo-client/src/core/ObservableQuery.ts
 */
[@bs.deriving abstract]
type updateQueryOptions('a) = {
  [@bs.optional]
  fetchMoreResult: option('a),
  [@bs.optional]
  variables: Js.Json.t,
};

type updateQueryT('a) = ('a, updateQueryOptions('a)) => 'a;

/**
 * https://github.com/apollographql/apollo-client/blob/master/packages/apollo-client/src/core/watchQueryOptions.ts#L139
 */
type updateSubscriptionOptionsJs = {
  .
  "subscriptionData": {. "data": Js.Json.t},
  "variables": Js.Nullable.t(Js.Json.t),
};

type updateQuerySubscribeToMoreT =
  (Js.Json.t, updateSubscriptionOptionsJs) => Js.Json.t;

[@bs.deriving abstract]
type subscribeToMoreOptionsJs = {
  document: ReasonApolloTypes.queryString,
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  updateQuery: updateQuerySubscribeToMoreT,
};

type unsubscribeFnT = unit => unit;

type refetch('a) = (~variables: Js.Json.t=?, unit) => Js.Promise.t('a);
type queryResult('a) = {
  data: option('a),
  loading: bool,
  error: option(apolloError),
  refetch: refetch('a),
  fetchMore:
    (~variables: Js.Json.t=?, ~updateQuery: updateQueryT('a), unit) =>
    Js.Promise.t(unit),
  networkStatus: ApolloHooksTypes.networkStatus,
  startPolling: int => unit,
  stopPolling: unit => unit,
  subscribeToMore:
    (
      ~document: ReasonApolloTypes.queryString,
      ~variables: Js.Json.t=?,
      ~updateQuery: updateQuerySubscribeToMoreT=?,
      unit
    ) =>
    unsubscribeFnT,
};

/**
 * apollo-client/src/core/watchQueryOptions.ts
 */
[@bs.deriving abstract]
type fetchMoreOptions('a) = {
  [@bs.optional]
  variables: Js.Json.t,
  updateQuery: updateQueryT('a),
};

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

[@bs.deriving abstract]
type options('raw_t) = {
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  client: ApolloClient.generatedApolloClient('raw_t),
  [@bs.optional]
  notifyOnNetworkStatusChange: bool,
  [@bs.optional]
  fetchPolicy: string,
  [@bs.optional]
  errorPolicy: string,
  [@bs.optional]
  skip: bool,
  [@bs.optional]
  pollInterval: int,
  [@bs.optional]
  context: Context.t,
};

type refetchResult('raw_t) = {data: Js.Nullable.t('raw_t)};

[@bs.module "@apollo/client"]
external useQueryJs:
  (ReasonApolloTypes.queryString, options('raw_t)) =>
  {
    .
    "data": Js.Nullable.t('raw_t),
    "loading": bool,
    "error": Js.Nullable.t(apolloError),
    [@bs.meth]
    "refetch":
      Js.Nullable.t(Js.Json.t) => Js.Promise.t(refetchResult('raw_t)),
    [@bs.meth] "fetchMore": fetchMoreOptions('a) => Js.Promise.t(unit),
    "networkStatus": Js.Nullable.t(int),
    [@bs.meth] "stopPolling": unit => unit,
    [@bs.meth] "startPolling": int => unit,
    [@bs.meth] "subscribeToMore": subscribeToMoreOptionsJs => unsubscribeFnT,
  } =
  "useQuery";

let useQuery:
  (
    ~client: ApolloClient.generatedApolloClient('raw_t)=?,
    ~variables: Js.Json.t=?,
    ~notifyOnNetworkStatusChange: bool=?,
    ~fetchPolicy: ApolloHooksTypes.fetchPolicy=?,
    ~errorPolicy: ApolloHooksTypes.errorPolicy=?,
    ~skip: bool=?,
    ~pollInterval: int=?,
    ~context: Context.t=?,
    graphqlDefinition('t, 'raw_t, _, _)
  ) =>
  (variant('t), queryResult('t)) =
  (
    ~client=?,
    ~variables=?,
    ~notifyOnNetworkStatusChange=?,
    ~fetchPolicy=?,
    ~errorPolicy=?,
    ~skip=?,
    ~pollInterval=?,
    ~context=?,
    (parse, query, _),
  ) => {
    let jsResult =
      useQueryJs(
        gql(. query),
        options(
          ~variables?,
          ~client?,
          ~notifyOnNetworkStatusChange?,
          ~fetchPolicy=?
            fetchPolicy->Belt.Option.map(ApolloHooksTypes.fetchPolicyToJs),
          ~errorPolicy=?
            errorPolicy->Belt.Option.map(ApolloHooksTypes.errorPolicyToJs),
          ~skip?,
          ~pollInterval?,
          ~context?,
          (),
        ),
      );

    let result =
      React.useMemo1(
        () =>
          {
            data:
              jsResult##data
              ->Js.Nullable.toOption
              ->Belt.Option.flatMap(data =>
                  switch (parse(data)) {
                  | parsedData => Some(parsedData)
                  | exception _ => None
                  }
                ),
            loading: jsResult##loading,
            error: jsResult##error->Js.Nullable.toOption,
            networkStatus:
              ApolloHooksTypes.toNetworkStatus(jsResult##networkStatus),
            refetch: (~variables=?, ()) =>
              jsResult##refetch(Js.Nullable.fromOption(variables))
              |> Js.Promise.then_(result =>
                   parse(
                     result.data->Js.Nullable.toOption->Belt.Option.getExn,
                   )
                   |> Js.Promise.resolve
                 ),
            fetchMore: (~variables=?, ~updateQuery, ()) =>
              jsResult##fetchMore(
                fetchMoreOptions(~variables?, ~updateQuery, ()),
              ),
            stopPolling: () => jsResult##stopPolling(),
            startPolling: interval => jsResult##startPolling(interval),
            subscribeToMore: (~document, ~variables=?, ~updateQuery=?, ()) =>
              jsResult##subscribeToMore(
                subscribeToMoreOptionsJs(
                  ~document,
                  ~variables?,
                  ~updateQuery?,
                  (),
                ),
              ),
          },
        [|jsResult|],
      );

    let simple =
      React.useMemo1(
        () =>
          switch (result) {
          | {loading: true} => Loading
          | {error: Some(error)} => Error(error)
          | {data: Some(data)} => Data(data)
          | _ => NoData
          },
        [|result|],
      );

    (simple, result);
  };
