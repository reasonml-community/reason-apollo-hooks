module ErrorPolicy = {
  type t =
    | None
    | Ignore
    | All;

  let toRaw =
    fun
    | None => "none"
    | Ignore => "ignore"
    | All => "all";
};

module WatchQueryFetchPolicy = {
  type t =
    | CacheAndNetwork
    // ...extends FetchPolicy
    | CacheFirst
    | NetworkOnly
    | CacheOnly
    | NoCache
    | Standby;

  let toRaw =
    fun
    | CacheFirst => "cache-first"
    | CacheAndNetwork => "cache-and-network"
    | NetworkOnly => "network-only"
    | CacheOnly => "cache-only"
    | NoCache => "no-cache"
    | Standby => "standby";
};
