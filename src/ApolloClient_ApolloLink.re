module Types = ApolloClient_Types;

type t = Types.apolloLink;
type fetch;
type request;
type context;

type linkOptions = {
  uri: string,
  includeExtensions: Js.Nullable.t(bool),
  fetch: Js.Nullable.t(fetch),
  headers: Js.Nullable.t(Js.Json.t),
  credentials: Js.Nullable.t(string),
  fetchOptions: Js.Nullable.t(Js.Json.t),
};

type fetchResult('raw_t) = {
  errors: Js.Nullable.t(Js.Array.t(Types.graphqlError)),
  data: Js.Nullable.t('raw_t),
  extensions: Js.Json.t,
  context: Js.Json.t,
};

type nextLink('raw_t, 'raw_t_variables) =
  Types.operation('raw_t_variables) =>
  Types.Observable.t(fetchResult('raw_t));

type makeCb('raw_t, 'raw_t_variables) =
  (Types.operation('raw_t_variables), nextLink('raw_t, 'raw_t_variables)) =>
  Types.Observable.t(fetchResult('raw_t));
external make: makeCb('raw_t, 'raw_t_variables) => t;

/* Bind the method `from`, used to compose links together */
[@bs.module "@apollo/client"] external from: array(t) => t = "from";

/* Bind the method split. Based on a test send left or right */
[@bs.module "@apollo/client"]
external split: (Types.splitTest => bool, t, t) => t = "split";

module RetryLink = {
  // serverError is an Error instance with some extra props
  // see: https://github.com/apollographql/apollo-client/blob/master/src/link/utils/throwServerError.ts
  type serverError = {
    statusCode: int,
    response: Js.Json.t,
    result: Js.Json.t,
  };
  type delay = {
    initial: int,
    max: int,
    jitter: bool,
  };
  type attempts('raw_t_variables) = {
    max: int,
    retryIf:
      (Js.Nullable.t(serverError), Types.operation('raw_t_variables)) => bool,
  };
  type options('raw_t_variables) = {
    delay,
    attempts: attempts('raw_t_variables),
  };

  [@bs.module "apollo-link-retry"] [@bs.new]
  external make: options('raw_t_variables) => t = "RetryLink";
};

module HttpLink = {
  [@bs.module "@apollo/client"] [@bs.new]
  external jsMake: linkOptions => t = "HttpLink";
  let make =
      (
        ~uri,
        ~includeExtensions=?,
        ~fetch=?,
        ~headers=?,
        ~credentials=?,
        ~fetchOptions=?,
        (),
      ) => {
    jsMake({
      uri,
      includeExtensions: Js.Nullable.fromOption(includeExtensions),
      fetch: Js.Nullable.fromOption(fetch),
      headers: Js.Nullable.fromOption(headers),
      credentials: Js.Nullable.fromOption(credentials),
      fetchOptions: Js.Nullable.fromOption(fetchOptions),
    });
  };
};

module LinkContext = {
  /* Bind the setContext method */
  [@bs.module "apollo-link-context"]
  external setContext: ([@bs.uncurry] ((request, context) => Js.Json.t)) => t =
    "setContext";

  [@bs.module "apollo-link-context"]
  external setContextPromise:
    ((request, context) => Js.Promise.t(Js.Json.t)) => t =
    "setContext";
};

module LinkError = {
  /* Bind the onError method */
  [@bs.module "apollo-link-error"]
  external onError:
    (Types.errorResponse('raw_t, 'raw_t_variables) => unit) => t =
    "onError";
};

/* bind apollo-link-ws */
[@bs.module "apollo-link-ws"] [@bs.new]
external webSocketLink: Types.webSocketLink => t = "WebSocketLink";

// module UploadLink = {
//   type uploadLinkOptions = {
//     uri: Js.Nullable.t(string),
//     fetch: Js.Nullable.t(fetch),
//     fetchOptions: Js.Nullable.t(Js.t({.})),
//     credentials: Js.Nullable.t(string),
//     headers: Js.Nullable.t(Js.Json.t),
//     includeExtensions: Js.Nullable.t(bool),
//   };

//   /* Bind createUploadLink function from apollo upload link */
//   [@bs.module "apollo-upload-client"]
//   external jsMake: uploadLinkOptions => t = "createUploadLink";
//   let make =
//       (
//         ~uri=?,
//         ~fetch=?,
//         ~fetchOptions=?,
//         ~credentials=?,
//         ~headers=?,
//         ~includeExtensions=?,
//         (),
//       ) =>
//     jsMake(
//       Js.Nullable.{
//         uri: fromOption(uri),
//         fetch: fromOption(fetch),
//         fetchOptions: fromOption(fetchOptions),
//         credentials: fromOption(credentials),
//         headers: fromOption(headers),
//         includeExtensions: fromOption(includeExtensions),
//       },
//     );
// };
