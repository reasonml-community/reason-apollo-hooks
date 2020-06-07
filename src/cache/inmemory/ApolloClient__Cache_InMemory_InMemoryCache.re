module KeyFieldsFunction = ApolloClient__Cache_InMemory_Policies.KeyFieldsFunction;
module PossibleTypesMap = ApolloClient__Cache_InMemory_Policies.PossibleTypesMap;
module TypePolicies = ApolloClient__Cache_InMemory_Policies.TypePolicies;

module InMemoryCacheConfig = {
  module Js_ = {
    // export interface InMemoryCacheConfig extends ApolloReducerConfig {
    //     resultCaching?: boolean;
    //     possibleTypes?: PossibleTypesMap;
    //     typePolicies?: TypePolicies;
    // }
    // NOTE: Using deriving abstract here because passing properties that are undefined has effects
    [@bs.deriving abstract]
    type t = {
      [@bs.optional]
      resultCaching: bool,
      [@bs.optional]
      possibleTypes: PossibleTypesMap.Js_.t,
      [@bs.optional]
      typePolicies: TypePolicies.Js_.t,
      // ...extends ApolloReducerConfig
      [@bs.optional]
      dataIdFromObject: KeyFieldsFunction.Js_.t,
      [@bs.optional]
      addTypename: bool,
    };
  };
  type t = Js_.t;
  let make = Js_.t;
};

module Js_ = {
  // export declare class InMemoryCache extends ApolloCache<NormalizedCacheObject> {
  //     private data;
  //     private optimisticData;
  //     protected config: InMemoryCacheConfig;
  //     private watches;
  //     private addTypename;
  //     private typenameDocumentCache;
  //     private storeReader;
  //     private storeWriter;
  //     readonly policies: Policies;
  //     constructor(config?: InMemoryCacheConfig);
  //     restore(data: NormalizedCacheObject): this;
  //     extract(optimistic?: boolean): NormalizedCacheObject;
  //     read<T>(options: Cache.ReadOptions): T | null;
  //     write(options: Cache.WriteOptions): void;
  //     diff<T>(options: Cache.DiffOptions): Cache.DiffResult<T>;
  //     watch(watch: Cache.WatchOptions): () => void;
  //     gc(): string[];
  //     retain(rootId: string, optimistic?: boolean): number;
  //     release(rootId: string, optimistic?: boolean): number;
  //     identify(object: StoreObject): string | undefined;
  //     evict(idOrOptions: string | Cache.EvictOptions, fieldName?: string, args?: Record<string, any>): boolean;
  //     reset(): Promise<void>;
  //     removeOptimistic(idToRemove: string): void;
  //     private txCount;
  //     performTransaction(transaction: (cache: InMemoryCache) => any, optimisticId?: string): void;
  //     recordOptimisticTransaction(transaction: Transaction<NormalizedCacheObject>, id: string): void;
  //     transformDocument(document: DocumentNode): DocumentNode;
  //     protected broadcastWatches(): void;
  //     private maybeBroadcastWatch;
  //     private varDep;
  //     makeVar<T>(value: T): ReactiveVar<T>;
  // }
  type t = ApolloClient__Cache_Core_Cache.ApolloCache.Js_.t(Js.Json.t);

  [@bs.module "@apollo/client"] [@bs.new]
  external make: InMemoryCacheConfig.Js_.t => t = "InMemoryCache";
};

type t = Js_.t;

let make:
  (
    ~resultCaching: bool=?,
    ~possibleTypes: PossibleTypesMap.t=?,
    ~typePolicies: TypePolicies.t=?,
    ~dataIdFromObject: KeyFieldsFunction.t=?,
    ~addTypename: bool=?,
    unit
  ) =>
  t =
  (
    ~resultCaching=?,
    ~possibleTypes=?,
    ~typePolicies=?,
    ~dataIdFromObject=?,
    ~addTypename=?,
    (),
  ) => {
    Js_.make(
      InMemoryCacheConfig.make(
        ~resultCaching?,
        ~possibleTypes?,
        ~typePolicies?,
        ~dataIdFromObject?,
        ~addTypename?,
        (),
      ),
    );
  };
