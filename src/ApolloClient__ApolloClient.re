module ApolloQueryResult = ApolloClient__Core_Types.ApolloQueryResult;
module DataProxy = ApolloClient__Cache_Core_Types.DataProxy;
module ErrorPolicy = ApolloClient__Core_WatchQueryOptions.ErrorPolicy;
module FetchPolicy = ApolloClient__Core_WatchQueryOptions.FetchPolicy;
module FetchPolicy__noCacheExtracted = ApolloClient__Core_WatchQueryOptions.FetchPolicy__noCacheExtracted;
module GraphQL = ApolloClient__Graphql;
module GraphqlTag = ApolloClient__GraphqlTag;
module FetchResult = ApolloClient__Link_Core_Types.FetchResult;
module MutationOptions = ApolloClient__Core_WatchQueryOptions.MutationOptions;
module MutationQueryReducersMap = ApolloClient__Core_WatchQueryOptions.MutationQueryReducersMap;
module MutationUpdaterFn = ApolloClient__Core_WatchQueryOptions.MutationUpdaterFn;
module QueryOptions = ApolloClient__Core_WatchQueryOptions.QueryOptions;
module PureQueryOptions = ApolloClient__Core_Types.PureQueryOptions;
module RefetchQueryDescription = ApolloClient__Core_WatchQueryOptions.RefetchQueryDescription;
module Types = ApolloClient__Types;

// /// <reference types="zen-observable" />

// export interface DefaultOptions {
//     watchQuery?: Partial<WatchQueryOptions>;
//     query?: Partial<QueryOptions>;
//     mutate?: Partial<MutationOptions>;
// }

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
//     constructor(options: ApolloClientOptions<TCacheShape>);
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

type refetchQueryDescription =
  | PureQueryOptions(PureQueryOptions.t('variables)): refetchQueryDescription
  | String(string): refetchQueryDescription;

module Js_ = {
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
};

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
    (module Definition),
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
            mutation: GraphqlTag.gql(Definition.query),
            optimisticResponse,
            updateQueries,
            refetchQueries,
            update,
            variables,
          },
          ~parse=Definition.parse,
          ~serialize=Definition.serialize,
        ),
    )
    ->Js.Promise.then_(
        jsResult =>
          jsResult
          ->FetchResult.fromJs(_, ~parse=Definition.parse)
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
    (module Definition),
  ) => {
    Js_.query(
      client,
      ~options=
        QueryOptions.toJs({
          fetchPolicy,
          query: GraphqlTag.gql(Definition.query),
          variables,
          errorPolicy,
          context,
        }),
    )
    ->Js.Promise.then_(
        jsResult =>
          jsResult
          ->ApolloQueryResult.fromJs(_, ~parse=Definition.parse)
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
  (client, ~id=?, ~optimistic=?, ~variables=?, (module Definition)) => {
    Js_.readQuery(
      client,
      ~options={id, query: GraphqlTag.gql(Definition.query), variables},
      ~optimistic,
    )
    ->Js.toOption
    ->Belt.Option.map(Definition.parse);
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
  (client, ~broadcast=?, ~data, ~id=?, ~variables=?, (module Definition)) => {
    Js_.writeQuery(
      client,
      ~options={
        broadcast,
        data,
        id,
        query: GraphqlTag.gql(Definition.query),
        variables,
      },
    );
  };
