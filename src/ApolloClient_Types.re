/**
 * An abstract type to describe a query string object.
 */
type queryString;

/**
 * The signature of the `graphql-tag/gql` function that transforms a GraphQL
 * query string to the standard GraphQL AST.
 * https://github.com/apollographql/graphql-tag
 */
type gql = (. string) => queryString;

/**
 * An abstract type to describe an Apollo Link object.
 */
type apolloLink;

type documentDefinition = {
  kind: string,
  operation: string,
};
type documentNode = {definitions: array(documentDefinition)};

/**
 * An abstract type to describe an Apollo Cache object.
 */
type apolloCache;

type networkError = {statusCode: int};

type apolloErrorExtensions = {code: Js.Nullable.t(string)};

module Observable = {
  type t('t);
  type subscriptionObserver('t) = {
    closed: bool,
    next: 't => unit,
    error: Js.Json.t => unit,
    complete: unit => unit,
  };

  type subscription = {
    closed: bool,
    unsubscribe: unit => unit,
  };

  [@bs.send]
  external subscribe: (t('t), subscriptionObserver('t)) => subscription =
    "subscribe";

  type subscriptionObserverCb = unit => unit;
  [@bs.module "@apollo/client"] [@bs.new]
  external make: subscriptionObserver('t) => subscriptionObserverCb =
    "Observable";
};

type graphqlError = {
  message: string,
  name: Js.Nullable.t(string),
  extensions: Js.Nullable.t(apolloErrorExtensions),
  locations: Js.Nullable.t(array(string)),
  path: Js.Nullable.t(array(string)),
  nodes: Js.Nullable.t(array(string)),
};

type rawExecutionResult('raw_t) = {
  errors: Js.Nullable.t(Js.Array.t(graphqlError)),
  data: Js.Nullable.t('raw_t),
};

type apolloError = {
  graphQLErrors: Js.Nullable.t(array(graphqlError)),
  networkError: Js.Nullable.t(string),
};

type operation('raw_t_variables) = {
  query: documentNode,
  variables: 'raw_t_variables,
  operationName: string,
  extensions: Js.Json.t,
  setContext: Js.Json.t => Js.Json.t,
  getContext: unit => Js.Json.t,
};

type errorResponse('raw_t, 'raw_t_variables) = {
  graphQLErrors: Js.Nullable.t(Js.Array.t(graphqlError)),
  networkError: Js.Nullable.t(networkError),
  response: Js.Nullable.t(rawExecutionResult('raw_t)),
  operation: operation('raw_t_variables),
  forward: operation('raw_t_variables) => Observable.subscription,
};

/*
 apollo link ws
 */

type webSocketLinkOptions = {
  reconnect: bool,
  connectionParams: option(Js.Json.t),
};

type webSocketLink = {
  uri: string,
  options: webSocketLinkOptions,
};

type splitTest = {query: documentNode};

module type Definition = {
  let query: string;

  module Raw: {
    type t;
    type t_variables;
  };
  type t;

  let parse: Raw.t => t;
  let serialize: t => Raw.t;
};

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

module Context = {
  type t = Js.Dict.t(string);
  let make = (context): t => Js.Dict.fromList(context);
};

module type Operation = {
  let query: string;

  module Raw: {
    type t;
    type t_variables;
  };
  type t;
  type t_variables;

  let parse: Raw.t => t;
  let serialize: t => Raw.t;
  let serializeVariables: t_variables => Raw.t_variables;
};

module type OperationNoRequiredVars = {
  include Operation;
  let makeDefaultVariables: unit => t_variables;
};
