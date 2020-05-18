module ErrorPolicy = {
  [@bs.deriving jsConverter]
  type t = [ | `none | `ignore | `all];

  let toJs = tToJs;

  let fromJs = string => tFromJs(string)->Belt.Option.getExn;
};

module WatchQueryFetchPolicy = {
  [@bs.deriving jsConverter]
  type t = [
    | [@bs.as "cache-and-network"] `cacheAndNetwork
    | [@bs.as "cache-first"] `cacheFirst
    | [@bs.as "cache-only"] `cacheOnly
    | [@bs.as "network-only"] `networkOnly
    | [@bs.as "no-cache"] `noCache
    | `standby
  ];

  let toJs = tToJs;

  let fromJs = string => tFromJs(string)->Belt.Option.getExn;
};
