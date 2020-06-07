module ApolloQueryResult = ApolloClient__Core_Types.ApolloQueryResult;
module ApolloLink = ApolloClient__Link_Core_ApolloLink;
module ApolloCache = ApolloClient__Cache_Core_Cache.ApolloCache;
module DataProxy = ApolloClient__Cache_Core_Types.DataProxy;
module ErrorPolicy = ApolloClient__Core_WatchQueryOptions.ErrorPolicy;
module FetchPolicy = ApolloClient__Core_WatchQueryOptions.FetchPolicy;
module FetchPolicy__noCacheExtracted = ApolloClient__Core_WatchQueryOptions.FetchPolicy__noCacheExtracted;
module FragmentMatcher = ApolloClient__Core_LocalState.FragmentMatcher;
module GraphQL = ApolloClient__Graphql;
module GraphqlTag = ApolloClient__GraphqlTag;
module FetchResult = ApolloClient__Link_Core_Types.FetchResult;
module MutationOptions = ApolloClient__Core_WatchQueryOptions.MutationOptions;
module MutationQueryReducersMap = ApolloClient__Core_WatchQueryOptions.MutationQueryReducersMap;
module MutationUpdaterFn = ApolloClient__Core_WatchQueryOptions.MutationUpdaterFn;
module QueryOptions = ApolloClient__Core_WatchQueryOptions.QueryOptions;
module PureQueryOptions = ApolloClient__Core_Types.PureQueryOptions;
module RefetchQueryDescription = ApolloClient__Core_WatchQueryOptions.RefetchQueryDescription;
module Resolvers = ApolloClient__Core_Types.Resolvers;
module UriFunction = ApolloClient_Link_Http_SelectHttpOptionsAndBody.UriFunction;
module Types = ApolloClient__Reason_Types;
module WatchQueryFetchPolicy = ApolloClient__Core_WatchQueryOptions.WatchQueryFetchPolicy;
module WatchQueryOptions = ApolloClient__Core_WatchQueryOptions.QueryOptions;

module DefaultOptions = {
  module DefaultWatchQueryOptions = {
    module Js_ = {
      // Partial<QueryOptions>;
      type t = {
        fetchPolicy: option(WatchQueryFetchPolicy.Js_.t),
        // query: GraphQL.Language.documentNode,
        // variables: option('variables),
        errorPolicy: option(ErrorPolicy.Js_.t),
        context: option(Js.Json.t),
      };
    };

    type t = {
      fetchPolicy: option(WatchQueryFetchPolicy.t),
      errorPolicy: option(ErrorPolicy.t),
      context: option(Js.Json.t),
    };

    let toJs: t => Js_.t =
      t => {
        fetchPolicy:
          t.fetchPolicy->Belt.Option.map(WatchQueryFetchPolicy.toJs),
        errorPolicy: t.errorPolicy->Belt.Option.map(ErrorPolicy.toJs),
        context: t.context,
      };
  };

  module DefaultQueryOptions = {
    module Js_ = {
      // Partial<QueryOptions>;
      type t = {
        fetchPolicy: option(FetchPolicy.Js_.t),
        // query: GraphQL.Language.documentNode,
        // variables: option('variables),
        errorPolicy: option(ErrorPolicy.Js_.t),
        context: option(Js.Json.t),
      };
    };

    type t = {
      fetchPolicy: option(FetchPolicy.t),
      errorPolicy: option(ErrorPolicy.t),
      context: option(Js.Json.t),
    };

    let toJs: t => Js_.t =
      t => {
        fetchPolicy: t.fetchPolicy->Belt.Option.map(FetchPolicy.toJs),
        errorPolicy: t.errorPolicy->Belt.Option.map(ErrorPolicy.toJs),
        context: t.context,
      };
  };

  module DefaultMutationOptions = {
    module Js_ = {
      // Partial<MutationOptions>;
      type t = {
        context: option(Js.Json.t),
        fetchPolicy: option(FetchPolicy__noCacheExtracted.Js_.t),
        awaitRefetchQueries: option(bool),
        errorPolicy: option(ErrorPolicy.Js_.t),
        // optimisticResponse: option('variables => 'jsData),
        // update: option(MutationUpdaterFn.Js_.t('jsData)),
        // updateQueries: option(MutationQueryReducersMap.Js_.t('jsData)),
        refetchQueries: option(RefetchQueryDescription.Js_.t),
        // variables: option('variables),
      };
    };

    type t = {
      context: option(Js.Json.t),
      fetchPolicy: option(FetchPolicy__noCacheExtracted.t),
      awaitRefetchQueries: option(bool),
      errorPolicy: option(ErrorPolicy.t),
      refetchQueries: option(RefetchQueryDescription.t),
    };

    let toJs: t => Js_.t =
      t => {
        context: t.context,
        fetchPolicy:
          t.fetchPolicy->Belt.Option.map(FetchPolicy__noCacheExtracted.toJs),
        awaitRefetchQueries: t.awaitRefetchQueries,
        errorPolicy: t.errorPolicy->Belt.Option.map(ErrorPolicy.toJs),
        refetchQueries:
          t.refetchQueries->Belt.Option.map(RefetchQueryDescription.toJs),
      };

    let make =
        (
          ~context=?,
          ~fetchPolicy=?,
          ~awaitRefetchQueries=?,
          ~errorPolicy=?,
          ~refetchQueries=?,
          (),
        ) => {
      context,
      fetchPolicy,
      awaitRefetchQueries,
      errorPolicy,
      refetchQueries,
    };
  };

  module Js_ = {
    // export interface DefaultOptions {
    //     watchQuery?: Partial<WatchQueryOptions>;
    //     query?: Partial<QueryOptions>;
    //     mutate?: Partial<MutationOptions>;
    // }
    type t = {
      watchQuery: option(DefaultWatchQueryOptions.Js_.t),
      query: option(DefaultQueryOptions.Js_.t),
      mutate: option(DefaultMutationOptions.Js_.t),
    };
  };

  type t = {
    watchQuery: option(DefaultWatchQueryOptions.t),
    query: option(DefaultQueryOptions.t),
    mutate: option(DefaultMutationOptions.t),
  };

  let toJs: t => Js_.t =
    t => {
      watchQuery:
        t.watchQuery->Belt.Option.map(DefaultWatchQueryOptions.toJs),
      query: t.query->Belt.Option.map(DefaultQueryOptions.toJs),
      mutate: t.mutate->Belt.Option.map(DefaultMutationOptions.toJs),
    };

  let make:
    (
      ~mutate: DefaultMutationOptions.t=?,
      ~query: DefaultQueryOptions.t=?,
      ~watchQuery: DefaultWatchQueryOptions.t=?,
      unit
    ) =>
    t =
    (~mutate=?, ~query=?, ~watchQuery=?, ()) => {watchQuery, query, mutate};
};

module ApolloClientOptions = {
  module Js_ = {
    // export declare type ApolloClientOptions<TCacheShape> = {
    //     uri?: string | UriFunction;
    //     credentials?: string;
    //     headers?: Record<string, string>;
    //     link?: ApolloLink;
    //     cache: ApolloCache<TCacheShape>;
    //     ssrForceFetchDelay?: number;
    //     ssrMode?: boolean;
    //     connectToDevTools?: boolean;
    //     queryDeduplication?: boolean;
    //     defaultOptions?: DefaultOptions;
    //     assumeImmutableResults?: boolean;
    //     resolvers?: Resolvers | Resolvers[];
    //     typeDefs?: string | string[] | DocumentNode | DocumentNode[];
    //     fragmentMatcher?: FragmentMatcher;
    //     name?: string;
    //     version?: string;
    // };
    type t = {
      uri: option(UriFunction.Js_.t),
      credentials: option(string),
      headers: option(Js.Dict.t(string)),
      link: option(ApolloLink.Js_.t),
      cache: ApolloCache.Js_.t(Js.Json.t),
      ssrForceFetchDelay: option(int),
      ssrMode: option(bool),
      connectToDevTools: option(bool),
      queryDeduplication: option(bool),
      defaultOptions: option(DefaultOptions.Js_.t),
      assumeImmutableResults: option(bool),
      resolvers: option(array(Resolvers.Js_.t)),
      typeDefs: option(array(GraphQL.documentNode)),
      fragmentMatcher: option(FragmentMatcher.Js_.t),
      name: option(string),
      version: option(string),
    };
  };

  type t = {
    uri: option(UriFunction.t),
    credentials: option(string),
    headers: option(Js.Dict.t(string)),
    link: option(ApolloLink.t),
    cache: ApolloCache.Js_.t(Js.Json.t),
    ssrForceFetchDelay: option(int),
    ssrMode: option(bool),
    connectToDevTools: option(bool),
    queryDeduplication: option(bool),
    defaultOptions: option(DefaultOptions.t),
    assumeImmutableResults: option(bool),
    resolvers: option(array(Resolvers.t)),
    typeDefs: option(array(GraphQL.documentNode)),
    fragmentMatcher: option(FragmentMatcher.t),
    name: option(string),
    version: option(string),
  };

  let toJs: t => Js_.t =
    t => {
      uri: t.uri,
      credentials: t.credentials,
      headers: t.headers,
      link: t.link,
      cache: t.cache,
      ssrForceFetchDelay: t.ssrForceFetchDelay,
      ssrMode: t.ssrMode,
      connectToDevTools: t.connectToDevTools,
      queryDeduplication: t.queryDeduplication,
      defaultOptions: t.defaultOptions->Belt.Option.map(DefaultOptions.toJs),
      assumeImmutableResults: t.assumeImmutableResults,
      resolvers: t.resolvers,
      typeDefs: t.typeDefs,
      fragmentMatcher: t.fragmentMatcher,
      name: t.name,
      version: t.version,
    };
};

type refetchQueryDescription =
  | PureQueryOptions(PureQueryOptions.t('variables)): refetchQueryDescription
  | String(string): refetchQueryDescription;

module Js_ = {
  // export declare class ApolloClient<TCacheShape> implements DataProxy {
  //     link: ApolloLink;
  //     cache: ApolloCache<TCacheShape>;
  //     disableNetworkFetches: boolean;
  //     version: string;
  //     queryDeduplication: boolean;
  //     defaultOptions: DefaultOptions;
  //     readonly typeDefs: ApolloClientOptions<TCacheShape>['typeDefs'];
  //     private queryManager;
  //     private devToolsHookCb;
  //     private resetStoreCallbacks;
  //     private clearStoreCallbacks;
  //     private localState;
  //     stop(): void;
  //     watchQuery<T = any, TVariables = OperationVariables>(options: WatchQueryOptions<TVariables>): ObservableQuery<T, TVariables>;
  //     subscribe<T = any, TVariables = OperationVariables>(options: SubscriptionOptions<TVariables>): Observable<FetchResult<T>>;
  //     readFragment<T = any, TVariables = OperationVariables>(options: DataProxy.Fragment<TVariables>, optimistic?: boolean): T | null;
  //     writeFragment<TData = any, TVariables = OperationVariables>(options: DataProxy.WriteFragmentOptions<TData, TVariables>): void;
  //     __actionHookForDevTools(cb: () => any): void;
  //     __requestRaw(payload: GraphQLRequest): Observable<ExecutionResult>;
  //     resetStore(): Promise<ApolloQueryResult<any>[] | null>;
  //     clearStore(): Promise<any[]>;
  //     onResetStore(cb: () => Promise<any>): () => void;
  //     onClearStore(cb: () => Promise<any>): () => void;
  //     reFetchObservableQueries(includeStandby?: boolean): Promise<ApolloQueryResult<any>[]>;
  //     extract(optimistic?: boolean): TCacheShape;
  //     restore(serializedState: TCacheShape): ApolloCache<TCacheShape>;
  //     addResolvers(resolvers: Resolvers | Resolvers[]): void;
  //     setResolvers(resolvers: Resolvers | Resolvers[]): void;
  //     getResolvers(): Resolvers;
  //     setLocalStateFragmentMatcher(fragmentMatcher: FragmentMatcher): void;
  // }
  type t;
  // mutate<T = any, TVariables = OperationVariables>(options: MutationOptions<T, TVariables>): Promise<FetchResult<T>>;
  [@bs.send]
  external mutate:
    (t, ~options: MutationOptions.Js_.t('jsData, 'variables)) =>
    Js.Promise.t(FetchResult.Js_.t('jsData)) =
    "mutate";

  // query<T = any, TVariables = OperationVariables>(options: QueryOptions<TVariables>): Promise<ApolloQueryResult<T>>;
  [@bs.send]
  external query:
    (t, ~options: QueryOptions.Js_.t('variables)) =>
    Js.Promise.t(ApolloQueryResult.Js_.t('jsData)) =
    "query";

  // readQuery<T = any, TVariables = OperationVariables>(options: DataProxy.Query<TVariables>, optimistic?: boolean): T | null;
  [@bs.send]
  external readQuery:
    (
      t,
      ~options: DataProxy.Query.Js_.t('variables),
      ~optimistic: option(bool)
    ) =>
    Js.nullable('jsData) =
    "readQuery";

  // writeQuery<TData = any, TVariables = OperationVariables>(options: DataProxy.WriteQueryOptions<TData, TVariables>): void;
  [@bs.send]
  external writeQuery:
    (t, ~options: DataProxy.WriteQueryOptions.Js_.t('jsData, 'variables)) =>
    unit =
    "writeQuery";

  [@bs.module "@apollo/client"] [@bs.new]
  external make: ApolloClientOptions.Js_.t => t = "ApolloClient";
};

type t = Js_.t;

let make:
  (
    ~uri: UriFunction.t=?,
    ~credentials: string=?,
    ~headers: Js.Dict.t(string)=?,
    ~link: ApolloLink.t=?,
    ~cache: ApolloCache.Js_.t(Js.Json.t),
    ~ssrForceFetchDelay: int=?,
    ~ssrMode: bool=?,
    ~connectToDevTools: bool=?,
    ~queryDeduplication: bool=?,
    ~defaultOptions: DefaultOptions.t=?,
    ~assumeImmutableResults: bool=?,
    ~resolvers: array(Resolvers.t)=?,
    ~typeDefs: array(GraphQL.documentNode)=?,
    ~fragmentMatcher: FragmentMatcher.t=?,
    ~name: string=?,
    ~version: string=?,
    unit
  ) =>
  t =
  (
    ~uri=?,
    ~credentials=?,
    ~headers=?,
    ~link=?,
    ~cache,
    ~ssrForceFetchDelay=?,
    ~ssrMode=?,
    ~connectToDevTools=?,
    ~queryDeduplication=?,
    ~defaultOptions=?,
    ~assumeImmutableResults=?,
    ~resolvers=?,
    ~typeDefs=?,
    ~fragmentMatcher=?,
    ~name=?,
    ~version=?,
    (),
  ) =>
    Js_.make(
      ApolloClientOptions.toJs({
        uri,
        credentials,
        headers,
        link,
        cache,
        ssrForceFetchDelay,
        ssrMode,
        connectToDevTools,
        queryDeduplication,
        defaultOptions,
        assumeImmutableResults,
        resolvers,
        typeDefs,
        fragmentMatcher,
        name,
        version,
      }),
    );

let mutate:
  type data variables jsData jsVariables.
    (
      t,
      ~awaitRefetchQueries: bool=?,
      ~context: Js.Json.t=?,
      ~errorPolicy: ErrorPolicy.t=?,
      ~fetchPolicy: FetchPolicy__noCacheExtracted.t=?,
      ~optimisticResponse: variables => data=?,
      ~refetchQueries: RefetchQueryDescription.t=?,
      ~updateQueries: MutationQueryReducersMap.t(data)=?,
      ~update: MutationUpdaterFn.t(data)=?,
      ~variables: variables=?,
      (module Types.Operation with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    Js.Promise.t(FetchResult.t(data)) =
  (
    client,
    ~awaitRefetchQueries=?,
    ~context=?,
    ~errorPolicy=?,
    ~fetchPolicy=?,
    ~optimisticResponse=?,
    ~refetchQueries=?,
    ~updateQueries=?,
    ~update=?,
    ~variables=?,
    (module Operation),
  ) => {
    Js_.mutate(
      client,
      ~options=
        MutationOptions.toJs(
          {
            awaitRefetchQueries,
            context,
            errorPolicy,
            fetchPolicy,
            mutation: GraphqlTag.gql(Operation.query),
            optimisticResponse,
            updateQueries,
            refetchQueries,
            update,
            variables,
          },
          ~parse=Operation.parse,
          ~serialize=Operation.serialize,
        ),
    )
    ->Js.Promise.then_(
        jsResult =>
          jsResult
          ->FetchResult.fromJs(_, ~parse=Operation.parse)
          ->Js.Promise.resolve,
        _,
      );
  };

let query:
  type data variables jsData jsVariables.
    (
      t,
      ~context: Js.Json.t=?,
      ~errorPolicy: ErrorPolicy.t=?,
      ~fetchPolicy: FetchPolicy.t=?,
      ~variables: variables=?,
      (module Types.Operation with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    Js.Promise.t(ApolloQueryResult.t(data)) =
  (
    client,
    ~context=?,
    ~errorPolicy=?,
    ~fetchPolicy=?,
    ~variables=?,
    (module Operation),
  ) => {
    Js_.query(
      client,
      ~options=
        QueryOptions.toJs({
          fetchPolicy,
          query: GraphqlTag.gql(Operation.query),
          variables,
          errorPolicy,
          context,
        }),
    )
    ->Js.Promise.then_(
        jsResult =>
          jsResult
          ->ApolloQueryResult.fromJs(_, ~parse=Operation.parse)
          ->Js.Promise.resolve,
        _,
      );
  };

let readQuery:
  type data variables jsData jsVariables.
    (
      t,
      ~id: string=?,
      ~optimistic: bool=?,
      ~variables: variables=?,
      (module Types.Operation with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    option(data) =
  (client, ~id=?, ~optimistic=?, ~variables=?, (module Operation)) => {
    Js_.readQuery(
      client,
      ~options={id, query: GraphqlTag.gql(Operation.query), variables},
      ~optimistic,
    )
    ->Js.toOption
    ->Belt.Option.map(Operation.parse);
  };

let writeQuery:
  type data variables jsData jsVariables.
    (
      t,
      ~broadcast: bool=?,
      ~data: data,
      ~id: string=?,
      ~variables: variables=?,
      (module Types.Operation with
         type t = data and
         type Raw.t = jsData and
         type Raw.t_variables = jsVariables)
    ) =>
    unit =
  (client, ~broadcast=?, ~data, ~id=?, ~variables=?, (module Operation)) => {
    Js_.writeQuery(
      client,
      ~options={
        broadcast,
        data: data->Operation.serialize,
        id,
        query: GraphqlTag.gql(Operation.query),
        variables,
      },
    );
  };
