open ApolloHooksTypes;

type variant('a) =
  | Data('a)
  | Error(apolloError)
  | Loading
  | NoData;

/**
 * https://github.com/apollographql/apollo-client/blob/master/packages/apollo-client/src/core/watchQueryOptions.ts#L139
 */
type updateSubscriptionOptionsJs('raw_t, 'raw_t_variables) = {
  .
  "subscriptionData": {. "data": 'raw_t},
  "variables": Js.Nullable.t('raw_t_variables),
};

type updateQuerySubscribeToMoreT('raw_t, 'raw_t_variables) =
  ('raw_t, updateSubscriptionOptionsJs('raw_t, 'raw_t_variables)) => 'raw_t;

[@bs.deriving abstract]
type subscribeToMoreOptionsJs('raw_t, 'raw_t_variables) = {
  document: ReasonApolloTypes.queryString,
  [@bs.optional]
  variables: 'raw_t_variables,
  [@bs.optional]
  updateQuery: updateQuerySubscribeToMoreT('raw_t, 'raw_t_variables),
};

type unsubscribeFnT = unit => unit;

type updateQueryOptions('t, 'raw_t_variables) = {
  fetchMoreResult: option('t),
  variables: option('raw_t_variables),
};

module Raw = {
  /**
   *
   *  apollo-client/src/core/ObservableQuery.ts
   */
  type updateQueryOptions('raw_t, 'raw_t_variables) = {
    fetchMoreResult: Js.Nullable.t('raw_t),
    variables: Js.Nullable.t('raw_t_variables),
  };

  /**
   * apollo-client/src/core/watchQueryOptions.ts
   */
  type fetchMoreOptions('raw_t, 'raw_t_variables) = {
    variables: option('raw_t_variables),
    updateQuery:
      ('raw_t, updateQueryOptions('raw_t, 'raw_t_variables)) => 'raw_t,
  };
};

type queryResult('t, 'raw_t, 'raw_t_variables) = {
  data: option('t),
  loading: bool,
  error: option(apolloError),
  networkStatus: ApolloHooksTypes.networkStatus,
  refetch: (~variables: 'raw_t_variables=?, unit) => Js.Promise.t('t),
  fetchMore:
    (
      ~variables: 'raw_t_variables=?,
      ~updateQuery: ('t, updateQueryOptions('t, 'raw_t_variables)) => 't,
      unit
    ) =>
    Js.Promise.t(unit),
  rawFetchMore:
    (
      ~variables: 'raw_t_variables=?,
      ~updateQuery: (
                      'raw_t,
                      Raw.updateQueryOptions('raw_t, 'raw_t_variables)
                    ) =>
                    'raw_t,
      unit
    ) =>
    Js.Promise.t(unit),
  startPolling: int => unit,
  stopPolling: unit => unit,
  subscribeToMore:
    (
      ~document: ReasonApolloTypes.queryString,
      ~variables: 'raw_t_variables=?,
      ~updateQuery: updateQuerySubscribeToMoreT('raw_t, 'raw_t_variables)=?,
      unit
    ) =>
    unsubscribeFnT,
};

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

[@bs.deriving abstract]
type options('raw_t_variables) = {
  [@bs.optional]
  variables: 'raw_t_variables,
  [@bs.optional]
  client: ApolloClient.t,
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
  (ReasonApolloTypes.queryString, options('raw_t_variables)) =>
  {
    .
    "data": Js.Nullable.t('raw_t),
    "loading": bool,
    "error": Js.Nullable.t(apolloError),
    [@bs.meth]
    "refetch":
      Js.Nullable.t('raw_t_variables) => Js.Promise.t(refetchResult('raw_t)),
    [@bs.meth]
    "fetchMore":
      Raw.fetchMoreOptions('raw_t, 'raw_t_variables) => Js.Promise.t(unit),
    "networkStatus": Js.Nullable.t(int),
    [@bs.meth] "stopPolling": unit => unit,
    [@bs.meth] "startPolling": int => unit,
    [@bs.meth]
    "subscribeToMore":
      subscribeToMoreOptionsJs('raw_t, 'raw_t_variables) => unsubscribeFnT,
  } =
  "useQuery";

let useQuery:
  (
    ~client: ApolloClient.t=?,
    ~variables: 'raw_t_variables=?,
    ~notifyOnNetworkStatusChange: bool=?,
    ~fetchPolicy: ApolloHooksTypes.fetchPolicy=?,
    ~errorPolicy: ApolloHooksTypes.errorPolicy=?,
    ~skip: bool=?,
    ~pollInterval: int=?,
    ~context: Context.t=?,
    graphqlDefinition('t, 'raw_t)
  ) =>
  (variant('t), queryResult('t, 'raw_t, 'raw_t_variables)) =
  (
    ~client=?,
    ~variables=?,
    ~notifyOnNetworkStatusChange=?,
    ~fetchPolicy=?,
    ~errorPolicy=?,
    ~skip=?,
    ~pollInterval=?,
    ~context=?,
    (parse, query, serialize),
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
            rawFetchMore: (~variables=?, ~updateQuery, ()) =>
              jsResult##fetchMore({Raw.variables, Raw.updateQuery}),
            fetchMore: (~variables=?, ~updateQuery, ()) => {
              jsResult##fetchMore({
                Raw.variables,
                Raw.updateQuery:
                  (previousResult, {fetchMoreResult, variables}) => {
                  let result =
                    updateQuery(
                      parse(previousResult),
                      {
                        fetchMoreResult:
                          switch (Js.Nullable.toOption(fetchMoreResult)) {
                          | None => None
                          | Some(fetchMoreResult) =>
                            Some(parse(fetchMoreResult))
                          },
                        variables: Js.Nullable.toOption(variables),
                      },
                    );
                  serialize(result);
                },
              });
            },
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
