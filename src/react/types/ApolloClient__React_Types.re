module ApolloError = ApolloClient__ApolloError;
module ApolloQueryResult = ApolloClient__Core_Types.ApolloQueryResult;
module ErrorPolicy = ApolloClient__Core_WatchQueryOptions.ErrorPolicy;
module FetchPolicy = ApolloClient__Core_WatchQueryOptions.FetchPolicy;
module Graphql = ApolloClient__Graphql;
module NetworkStatus = ApolloClient__Core_NetworkStatus;
module WatchQueryFetchPolicy = ApolloClient__Core_WatchQueryOptions.WatchQueryFetchPolicy;

module QueryHookOptions = {
  module Js_ = {
    // export interface QueryHookOptions<TData = any, TVariables = OperationVariables> extends QueryFunctionOptions<TData, TVariables> {
    //     query?: DocumentNode;
    // }
    type t('jsData, 'variables) = {
      query: option(Graphql.documentNode),
      // ...extends QueryFunctionOptions
      displayName: option(string),
      skip: option(bool),
      onCompleted: option('jsData => unit),
      onError: option(ApolloError.t => unit),
      // ..extends BaseQueryOptions
      client: option(ApolloClient__ApolloClient.t),
      context: option(Js.Json.t), // ACTUAL: Record<string, any>
      errorPolicy: option(string),
      fetchPolicy: option(string),
      notifyOnNetworkStatusChange: option(bool),
      partialRefetch: option(bool),
      pollInterval: option(int),
      // INTENTIONALLY IGNORED
      // returnPartialData: option(bool),
      ssr: option(bool),
      // Intentionally restricted to not be non-optional. `option(unit)` does not compile cleanly to `undefined`
      variables: 'variables,
    };
  };

  type t('data, 'variables) = {
    query: option(Graphql.documentNode),
    // ...extends QueryFunctionOptions
    displayName: option(string),
    skip: option(bool),
    // consider parsing?
    onCompleted: option('data => unit),
    onError: option(ApolloError.t => unit),
    // ...extends BaseQueryOptions
    client: option(ApolloClient__ApolloClient.t),
    context: option(Js.Json.t),
    errorPolicy: option(ErrorPolicy.t),
    fetchPolicy: option(WatchQueryFetchPolicy.t),
    notifyOnNetworkStatusChange: option(bool),
    partialRefetch: option(bool),
    pollInterval: option(int),
    // INTENTIONALLY IGNORED
    // returnPartialData: option(bool),
    ssr: option(bool),
    variables: 'variables,
  };

  let toJs =
      (t: t('data, 'variables), ~parse: 'jsData => 'data)
      : Js_.t('jsData, 'variables) => {
    client: t.client,
    context: t.context,
    displayName: t.displayName,
    errorPolicy: t.errorPolicy->Belt.Option.map(ErrorPolicy.toJs),
    onCompleted:
      t.onCompleted
      ->Belt.Option.map((onCompleted, jsData) => onCompleted(jsData->parse)),
    onError: t.onError,
    fetchPolicy: t.fetchPolicy->Belt.Option.map(WatchQueryFetchPolicy.toJs),
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
  module Js_ = {
    type t_fetchMoreOptions_updateQueryOptions('parsedData, 'variables) = {
      fetchMoreResult: option('parsedData),
      variables: option('variables),
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

    // export interface QueryResult<TData = any, TVariables = OperationVariables> extends ObservableQueryFields<TData, TVariables> {
    //     client: ApolloClient<any>;
    //     data: TData | undefined;
    //     error?: ApolloError;
    //     loading: boolean;
    //     networkStatus: NetworkStatus;
    //     called: true;
    // }
    type t('jsData, 'variables) = {
      fetchMore:
        t_fetchMoreOptions('jsData, 'variables) =>
        Js.Promise.t(ApolloQueryResult.t('jsData)),
      called: bool,
      client: ApolloClient__ApolloClient.t,
      data: option('jsData),
      error: option(ApolloError.t),
      loading: bool,
      networkStatus: NetworkStatus.t,
    };
  };

  type t('parsedData, 'variables) = {
    called: bool,
    client: ApolloClient__ApolloClient.t,
    data: option('parsedData),
    error: option(ApolloError.t),
    fetchMore:
      (
        ~context: Js.Json.t=?,
        ~variables: 'variables=?,
        ~updateQuery: (
                        'parsedData,
                        Js_.t_fetchMoreOptions_updateQueryOptions(
                          'parsedData,
                          'variables,
                        )
                      ) =>
                      'parsedData,
        unit
      ) =>
      Js.Promise.t(ApolloQueryResult.t('parsedData)),
    loading: bool,
    networkStatus: NetworkStatus.t,
  };

  let fromJs:
    (
      Js_.t('jsData, 'variables),
      ~parse: 'jsData => 'parsedData,
      ~serialize: 'parsedData => 'jsData
    ) =>
    t('parsedData, 'variables) =
    (js, ~parse, ~serialize) => {
      called: js.called,
      client: js.client,
      data: js.data->Belt.Option.map(parse),
      error: js.error,
      fetchMore:
        (~context=?, ~variables=?, ~updateQuery as jsUpdateQuery, ()) => {
        js.fetchMore({
          context,
          query: None,
          updateQuery: (previousResult, {fetchMoreResult, variables}) =>
            jsUpdateQuery(
              parse(previousResult),
              {
                fetchMoreResult: fetchMoreResult->Belt.Option.map(parse),
                variables,
              },
            )
            ->serialize,
          variables,
        })
        ->Js.Promise.then_(
            jsResult =>
              Js.Promise.resolve(ApolloQueryResult.fromJs(jsResult, ~parse)),
            _,
          );
      },
      loading: js.loading,
      networkStatus: js.networkStatus,
    };
};

module SubscriptionResult = {
  module Js_ = {
    // export interface SubscriptionResult<TData = any> {
    //     loading: boolean;
    //     data?: TData;
    //     error?: ApolloError;
    // }
    type t('jsData) = {
      loading: bool,
      data: option('jsData),
      error: option(ApolloError.Js_.t),
    };
  };

  type t('data) = {
    loading: bool,
    data: option('data),
    error: option(ApolloError.Js_.t),
  };

  let fromJs: (Js_.t('jsData), ~parse: 'jsData => 'data) => t('data) =
    (js, ~parse) => {
      loading: js.loading,
      data: js.data->Belt.Option.map(parse),
      error: js.error,
    };
};

module OnSubscriptionDataOptions = {
  module Js_ = {
    // export interface OnSubscriptionDataOptions<TData = any> {
    //     client: ApolloClient<object>;
    //     subscriptionData: SubscriptionResult<TData>;
    // }
    type t('jsData) = {
      client: ApolloClient__ApolloClient.t,
      subscriptionData: SubscriptionResult.Js_.t('jsData),
    };
  };

  type t('data) = {
    client: ApolloClient__ApolloClient.t,
    subscriptionData: SubscriptionResult.t('data),
  };

  let fromJs: (Js_.t('jsData), ~parse: 'jsData => 'data) => t('data) =
    (js, ~parse) => {
      client: js.client,
      subscriptionData:
        js.subscriptionData->SubscriptionResult.fromJs(~parse),
    };
};

module BaseSubscriptionOptions = {
  module Js_ = {
    // export interface BaseSubscriptionOptions<TData = any, TVariables = OperationVariables> {
    //     variables?: TVariables;
    //     fetchPolicy?: FetchPolicy;
    //     shouldResubscribe?: boolean | ((options: BaseSubscriptionOptions<TData, TVariables>) => boolean);
    //     client?: ApolloClient<object>;
    //     skip?: boolean;
    //     onSubscriptionData?: (options: OnSubscriptionDataOptions<TData>) => any;
    //     onSubscriptionComplete?: () => void;
    // }
    type t('jsData, 'variables) = {
      variables: option('variables),
      fetchPolicy: option(FetchPolicy.t),
      shouldResubscribe: option((. t('jsData, 'variables)) => bool),
      client: option(ApolloClient__ApolloClient.t),
      skip: option(bool),
      onSubscriptionData:
        option((. OnSubscriptionDataOptions.Js_.t('jsData)) => unit),
      onSubscriptionComplete: option(unit => unit),
    };
  };

  type t('data, 'variables) = {
    variables: option('variables),
    fetchPolicy: option(FetchPolicy.t),
    shouldResubscribe: option(t('data, 'variables) => bool),
    client: option(ApolloClient__ApolloClient.t),
    skip: option(bool),
    onSubscriptionData: option(OnSubscriptionDataOptions.t('data) => unit),
    onSubscriptionComplete: option(unit => unit),
  };

  let fromJs: Js_.t('jsData, 'variables) => t('data, 'variables) =
    js => {
      variables: js.variables,
      fetchPolicy: js.fetchPolicy,
      // shouldResubscribe: what to do here?
      shouldResubscribe: Obj.magic(js.shouldResubscribe),
      client: js.client,
      skip: js.skip,
      // onSubscriptionData: what to do here?
      onSubscriptionData: Obj.magic(js.onSubscriptionData),
      onSubscriptionComplete: js.onSubscriptionComplete,
    };
};

module SubscriptionHookOptions = {
  module Js_ = {
    // export interface SubscriptionHookOptions<TData = any, TVariables = OperationVariables> extends BaseSubscriptionOptions<TData, TVariables> {
    //     subscription?: DocumentNode;
    // }
    type t('jsData, 'variables) = {
      subscription: option(Graphql.documentNode),
      // ...extends BaseSubscriptionOptions
      // Intentionally restricted to not be non-optional. `option(unit)` does not compile cleanly to `undefined`
      variables: 'variables,
      fetchPolicy: option(FetchPolicy.t),
      shouldResubscribe:
        option(
          (. BaseSubscriptionOptions.Js_.t('jsData, 'variables)) => bool,
        ),
      client: option(ApolloClient__ApolloClient.t),
      skip: option(bool),
      onSubscriptionData:
        option((. OnSubscriptionDataOptions.Js_.t('jsData)) => unit),
      onSubscriptionComplete: option(unit => unit),
    };
  };

  type t('data, 'variables) = {
    subscription: option(Graphql.documentNode),
    variables: 'variables,
    fetchPolicy: option(FetchPolicy.t),
    shouldResubscribe:
      option(BaseSubscriptionOptions.t('data, 'variables) => bool),
    client: option(ApolloClient__ApolloClient.t),
    skip: option(bool),
    onSubscriptionData: option(OnSubscriptionDataOptions.t('data) => unit),
    onSubscriptionComplete: option(unit => unit),
  };

  let toJs:
    (t('data, 'variables), ~parse: 'jsData => 'data) =>
    Js_.t('jsData, 'variables) =
    (t, ~parse) => {
      subscription: t.subscription,
      variables: t.variables,
      fetchPolicy: t.fetchPolicy,
      shouldResubscribe:
        t.shouldResubscribe
        ->Belt.Option.map(shouldResubscribe =>
            (. jsBaseSubscriptionOptions) =>
              shouldResubscribe(
                jsBaseSubscriptionOptions->BaseSubscriptionOptions.fromJs,
              )
          ),
      client: t.client,
      skip: t.skip,
      onSubscriptionData:
        t.onSubscriptionData
        ->Belt.Option.map(onSubscriptionData =>
            (. jsOnSubscriptionDataOptions) =>
              onSubscriptionData(
                jsOnSubscriptionDataOptions->OnSubscriptionDataOptions.fromJs(
                  ~parse,
                ),
              )
          ),
      onSubscriptionComplete: t.onSubscriptionComplete,
    };
};
