module Core = Apollo_Client__Core;
module Errors = Apollo_Client__Errors;
module Graphql = Apollo_Client__Graphql;

module QueryHookOptions = {
  module Raw = {
    type t('raw_tData, 'raw_tVariables) = {
      query: option(Graphql.documentNode),
      // ...extends QueryFunctionOptions
      displayName: option(string),
      skip: option(bool),
      onCompleted: option('raw_tData => unit),
      onError: option(Errors.ApolloError.t => unit),
      // ..extends BaseQueryOptions
      client: option(Apollo_Client__ApolloClient.t),
      context: option(Js.Json.t), // ACTUAL: Record<string, any>
      errorPolicy: option(string),
      fetchPolicy: option(string),
      notifyOnNetworkStatusChange: option(bool),
      pollInterval: option(int),
      ssr: option(bool),
      variables: option('raw_tVariables),
      partialRefetch: option(bool),
      // INTENTIONALLY IGNORED
      // returnPartialData: option(bool),
    };
  };

  type t('raw_tData, 'raw_tVariables) = {
    query: option(Graphql.documentNode),
    // ...extends QueryFunctionOptions
    displayName: option(string),
    skip: option(bool),
    // consider parsing?
    onCompleted: option('raw_tData => unit),
    onError: option(Errors.ApolloError.t => unit),
    // ...extends BaseQueryOptions
    client: option(Apollo_Client__ApolloClient.t),
    context: option(Js.Json.t),
    errorPolicy: option(Core.WatchQueryOptions.ErrorPolicy.t),
    fetchPolicy: option(Core.WatchQueryOptions.WatchQueryFetchPolicy.t),
    notifyOnNetworkStatusChange: option(bool),
    partialRefetch: option(bool),
    pollInterval: option(int),
    // INTENTIONALLY IGNORED
    // returnPartialData: option(bool),
    ssr: option(bool),
    variables: option('raw_tVariables),
  };

  let toRaw = (t: t(_, _)): Raw.t(_, _) => {
    client: t.client,
    context: t.context,
    displayName: t.displayName,
    errorPolicy:
      t.errorPolicy
      ->Belt.Option.map(Core.WatchQueryOptions.ErrorPolicy.toRaw),
    onCompleted: t.onCompleted,
    onError: t.onError,
    fetchPolicy:
      t.fetchPolicy
      ->Belt.Option.map(Core.WatchQueryOptions.WatchQueryFetchPolicy.toRaw),
    notifyOnNetworkStatusChange: t.notifyOnNetworkStatusChange,
    query: t.query,
    pollInterval: t.pollInterval,
    partialRefetch: t.partialRefetch,
    skip: t.skip,
    ssr: t.ssr,
    variables: t.variables,
  };
};

module QueryResult = {
  module Raw = {
    type t_fetchMoreOptions_updateQueryOptions('tData, 'raw_tVariables) = {
      fetchMoreResult: Js.nullable('tData),
      variables: Js.nullable('raw_tVariables),
    };

    type t_fetchMoreOptions('raw_tData, 'raw_tVariables) = {
      query: option(Graphql.Language.documentNode),
      variables: option('raw_tVariables),
      context: option(Js.Json.t),
      updateQuery:
        (
          'raw_tData,
          t_fetchMoreOptions_updateQueryOptions('raw_tData, 'raw_tVariables)
        ) =>
        'raw_tData,
    };

    type t('raw_tData, 'raw_tVariables) = {
      fetchMore:
        t_fetchMoreOptions('raw_tData, 'raw_tVariables) =>
        Js.Promise.t(Core.Types.ApolloQueryResult.Raw.t('raw_tData)),
      called: bool,
      client: Apollo_Client__ApolloClient.t,
      data: Js.nullable('raw_tData),
      error: Js.nullable(Errors.ApolloError.t),
      loading: bool,
      networkStatus: Core.NetworkStatus.t,
    };
  };

  type t_updateQueryOptions('tData, 'raw_tVariables) = {
    fetchMoreResult: option('tData),
    variables: option('raw_tVariables),
  };

  type t('tData, 'raw_tVariables) = {
    called: bool,
    client: Apollo_Client__ApolloClient.t,
    data: option('tData),
    error: option(Errors.ApolloError.t),
    fetchMore:
      (
        ~context: Js.Json.t=?,
        ~variables: 'raw_tVariables=?,
        ~updateQuery: (
                        'tData,
                        t_updateQueryOptions('tData, 'raw_tVariables)
                      ) =>
                      'tData,
        unit
      ) =>
      Js.Promise.t(Core.Types.ApolloQueryResult.t('tData)),
    loading: bool,
    networkStatus: Core.NetworkStatus.t,
  };

  let fromRaw:
    (
      Raw.t('raw_tData, 'raw_tVariables),
      ~parse: 'raw_tData => 'tData,
      ~serialize: 'tData => 'raw_tData
    ) =>
    t('tData, 'raw_tVariables) =
    (raw, ~parse, ~serialize) => {
      called: raw.called,
      client: raw.client,
      data: raw.data->Js.toOption->Belt.Option.map(parse),
      error: raw.error->Js.toOption,
      fetchMore:
        (~context=?, ~variables=?, ~updateQuery as jsUpdateQuery, ()) => {
        raw.fetchMore({
          context,
          query: None,
          updateQuery: (previousResult, {fetchMoreResult, variables}) =>
            jsUpdateQuery(
              parse(previousResult),
              {
                fetchMoreResult:
                  fetchMoreResult->Js.toOption->Belt.Option.map(parse),
                variables: variables->Js.toOption,
              },
            )
            ->serialize,
          variables,
        })
        ->Js.Promise.then_(
            jsResult =>
              Js.Promise.resolve(
                Core.Types.ApolloQueryResult.fromRaw(jsResult, ~parse),
              ),
            _,
          );
      },
      loading: raw.loading,
      networkStatus: raw.networkStatus,
    };
};
