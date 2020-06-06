module Operation = ApolloClient__Link_Core_Types.Operation;

// import { Operation } from '../core/types';
// export interface Body {
//     query?: string;
//     operationName?: string;
//     variables?: Record<string, any>;
//     extensions?: Record<string, any>;
// }

module UriFunction = {
  module Js_ = {
    type t = Operation.Js_.t => string;
  };

  type t = Js_.t;
};

module HttpOptions = {
  module Js_ = {
    // export interface HttpOptions {
    //     uri?: string | UriFunction;
    //     includeExtensions?: boolean;
    //     fetch?: WindowOrWorkerGlobalScope['fetch'];
    //     headers?: any;
    //     credentials?: string;
    //     fetchOptions?: any;
    //     useGETForQueries?: boolean;
    // }
    type t_fetch;

    type t = {
      uri: option(UriFunction.Js_.t),
      includeExtensions: option(bool),
      fetch: option(t_fetch),
      headers: option(Js.Json.t),
      credentials: option(string),
      fetchOptions: Js.Json.t,
      useGETForQueries: option(bool),
    };
  };

  type t = Js_.t;
};
// export interface HttpOptions {
//     uri?: string | UriFunction;
//     includeExtensions?: boolean;
//     fetch?: WindowOrWorkerGlobalScope['fetch'];
//     headers?: any;
//     credentials?: string;
//     fetchOptions?: any;
//     useGETForQueries?: boolean;
// }
// export interface HttpQueryOptions {
//     includeQuery?: boolean;
//     includeExtensions?: boolean;
// }
// export interface HttpConfig {
//     http?: HttpQueryOptions;
//     options?: any;
//     headers?: any;
//     credentials?: any;
// }
// export declare const fallbackHttpConfig: {
//     http: HttpQueryOptions;
//     headers: {
//         accept: string;
//         'content-type': string;
//     };
//     options: {
//         method: string;
//     };
// };
// export declare const selectHttpOptionsAndBody: (operation: Operation, fallbackConfig: HttpConfig, ...configs: Array<HttpConfig>) => {
//     options: HttpConfig & Record<string, any>;
//     body: Body;
// };

();
