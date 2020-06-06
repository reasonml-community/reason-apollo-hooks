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
    type t = {
      resultCaching: option(bool),
      possibleTypes: option(PossibleTypesMap.Js_.t),
      typePolicies: option(TypePolicies.Js_.t),
      // ...extends ApolloReducerConfig
      dataIdFromObject: option(KeyFieldsFunction.Js_.t),
      addTypename: option(bool),
    };
  };
  type t = Js_.t;
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
  type t;

  [@bs.module "@apollo/client"]
  external make: InMemoryCacheConfig.Js_.t => t = "InMemoryCache";
};

type t = Js_.t;

let make: InMemoryCacheConfig.t => t = Js_.make;
