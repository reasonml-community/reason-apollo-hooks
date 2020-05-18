module Core = Apollo_Client__Core;
module Errors = Apollo_Client__Errors;
module Graphql = Apollo_Client__Graphql;

module QueryHookOptions = {
  module JS = {
    type t('jsData, 'variables) = {
      query: option(Graphql.documentNode),
      // ...extends QueryFunctionOptions
      displayName: option(string),
      skip: option(bool),
      onCompleted: option('jsData => unit),
      onError: option(Errors.ApolloError.t => unit),
      // ..extends BaseQueryOptions
      client: option(Apollo_Client__ApolloClient.t),
      context: option(Js.Json.t), // ACTUAL: Record<string, any>
      errorPolicy: option(string),
      fetchPolicy: option(string),
      notifyOnNetworkStatusChange: option(bool),
      pollInterval: option(int),
      ssr: option(bool),
      variables: option('variables),
      partialRefetch: option(bool),
      // INTENTIONALLY IGNORED
      // returnPartialData: option(bool),
    };
  };

  type t('jsData, 'variables) = {
    query: option(Graphql.documentNode),
    // ...extends QueryFunctionOptions
    displayName: option(string),
    skip: option(bool),
    // consider parsing?
    onCompleted: option('jsData => unit),
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
    variables: option('variables),
  };

  let toJs = (t: t(_, _)): JS.t(_, _) => {
    client: t.client,
    context: t.context,
    displayName: t.displayName,
    errorPolicy:
      t.errorPolicy->Belt.Option.map(Core.WatchQueryOptions.ErrorPolicy.toJs),
    onCompleted: t.onCompleted,
    onError: t.onError,
    fetchPolicy:
      t.fetchPolicy
      ->Belt.Option.map(Core.WatchQueryOptions.WatchQueryFetchPolicy.toJs),
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
  module JS = {
    type t_fetchMoreOptions_updateQueryOptions('parsedData, 'variables) = {
      fetchMoreResult: Js.nullable('parsedData),
      variables: Js.nullable('variables),
    };

    type t_fetchMoreOptions('jsData, 'variables) = {
      query: option(Graphql.Language.documentNode),
      variables: option('variables),
      context: option(Js.Json.t),
      updateQuery:
        (
          'jsData,
          t_fetchMoreOptions_updateQueryOptions('jsData, 'variables)
        ) =>
        'jsData,
    };

    type t('jsData, 'variables) = {
      fetchMore:
        t_fetchMoreOptions('jsData, 'variables) =>
        Js.Promise.t(Core.Types.ApolloQueryResult.JS.t('jsData)),
      called: bool,
      client: Apollo_Client__ApolloClient.t,
      data: Js.nullable('jsData),
      error: Js.nullable(Errors.ApolloError.t),
      loading: bool,
      networkStatus: Core.NetworkStatus.t,
    };
  };

  type t_updateQueryOptions('parsedData, 'variables) = {
    fetchMoreResult: option('parsedData),
    variables: option('variables),
  };

  type t('parsedData, 'variables) = {
    called: bool,
    client: Apollo_Client__ApolloClient.t,
    data: option('parsedData),
    error: option(Errors.ApolloError.t),
    fetchMore:
      (
        ~context: Js.Json.t=?,
        ~variables: 'variables=?,
        ~updateQuery: (
                        'parsedData,
                        t_updateQueryOptions('parsedData, 'variables)
                      ) =>
                      'parsedData,
        unit
      ) =>
      Js.Promise.t(Core.Types.ApolloQueryResult.t('parsedData)),
    loading: bool,
    networkStatus: Core.NetworkStatus.t,
  };

  let fromJs:
    (
      JS.t('jsData, 'variables),
      ~parse: 'jsData => 'parsedData,
      ~serialize: 'parsedData => 'jsData
    ) =>
    t('parsedData, 'variables) =
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
                Core.Types.ApolloQueryResult.fromJs(jsResult, ~parse),
              ),
            _,
          );
      },
      loading: raw.loading,
      networkStatus: raw.networkStatus,
    };
};
