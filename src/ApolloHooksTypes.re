/**
 * apollo-client/src/core/networkStatus
 */
type networkStatus =
  | Loading
  | SetVariables
  | FetchMore
  | Refetch
  | Poll
  | Ready
  | Error
  | Unknown;

let toNetworkStatus = (status: Js.Nullable.t(int)) => {
  switch (status->Js.Nullable.toOption) {
  | Some(1) => Loading
  | Some(2) => SetVariables
  | Some(3) => FetchMore
  | Some(4) => Refetch
  | Some(6) => Poll
  | Some(7) => Ready
  | Some(8) => Error
  | _ => Unknown
  };
};

/**
 * apollo-client/src/core/watchQueryOptions.ts
 */
type fetchPolicy =
  | CacheFirst
  | CacheAndNetwork
  | NetworkOnly
  | CacheOnly
  | NoCache
  | Standby;

let fetchPolicyToJs = fetchPolicy => {
  switch (fetchPolicy) {
  | CacheFirst => "cache-first"
  | CacheAndNetwork => "cache-and-network"
  | NetworkOnly => "network-only"
  | CacheOnly => "cache-only"
  | NoCache => "no-cache"
  | Standby => "standby"
  };
};

/**
 * apollo-client/src/core/watchQueryOptions.ts
 */
type errorPolicy =
  | None
  | Ignore
  | All;

let errorPolicyToJs = errorPolicy =>
  switch (errorPolicy) {
  | None => "none"
  | Ignore => "ignore"
  | All => "all"
  };

/**
 * apollo-client/src/errors/ApolloError.ts
 */
type apolloErrorExtensions = {. "code": Js.Nullable.t(string)};

type graphqlError = {
  .
  "message": string,
  "name": Js.Nullable.t(string),
  "extensions": Js.Nullable.t(apolloErrorExtensions),
  "locations": Js.Nullable.t(array(string)),
  "path": Js.Nullable.t(array(string)),
  "nodes": Js.Nullable.t(array(string)),
};

type apolloError = {
  .
  "message": string,
  "graphQLErrors": Js.Nullable.t(array(graphqlError)),
  "networkError": Js.Nullable.t(string),
};

type parse('a) = Js.Json.t => 'a;
type query = string;
type composeVariables('returnType, 'hookReturnType) =
  (Js.Json.t => 'returnType) => 'hookReturnType;

type graphqlDefinition('data, 'returnType, 'hookReturnType) = (
  parse('data),
  query,
  composeVariables('returnType, 'hookReturnType),
);

module Context = {
  type t = Js.Dict.t(string);
  let make = (context): t => Js.Dict.fromList(context);
};
