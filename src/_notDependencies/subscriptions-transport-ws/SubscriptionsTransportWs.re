// import { ListenerFn } from 'eventemitter3';
// import { ExecutionResult } from 'graphql/execution/execute';
// import { DocumentNode } from 'graphql/language/ast';
// export interface Observer<T> {
//     next?: (value: T) => void;
//     error?: (error: Error) => void;
//     complete?: () => void;
// }
// export interface Observable<T> {
//     subscribe(observer: Observer<T>): {
//         unsubscribe: () => void;
//     };
// }
// export interface OperationOptions {
//     query?: string | DocumentNode;
//     variables?: Object;
//     operationName?: string;
//     [key: string]: any;
// }
// export declare type FormatedError = Error & {
//     originalError?: any;
// };
// export interface Operation {
//     options: OperationOptions;
//     handler: (error: Error[], result?: any) => void;
// }
// export interface Operations {
//     [id: string]: Operation;
// }
// export interface Middleware {
//     applyMiddleware(options: OperationOptions, next: Function): void;
// }
module ConnectionParams = {
  module Js_ = {
    // export declare type ConnectionParams = {
    //     [paramName: string]: any;
    // };
    type t = Js.Json.t;
  };

  type t = Js_.t;
};

module ConnectionParamsOptions = {
  module Js_ = {
    module Union: {
      type t;
      let connectionParams: ConnectionParams.Js_.t => t;
      let fn: (unit => ConnectionParams.Js_.t) => t;
      let promise: Js.Promise.t(ConnectionParams.Js_.t) => t;
    } = {
      [@unboxed]
      type t =
        | Any('a): t;
      let connectionParams = (v: ConnectionParams.Js_.t) => Any(v);
      let fn = (v: unit => ConnectionParams.Js_.t) => Any(v);
      let promise = (v: Js.Promise.t(ConnectionParams.Js_.t)) => Any(v);
    };
    // export declare type ConnectionParamsOptions = ConnectionParams | Function | Promise<ConnectionParams>;
    type t = Union.t;
  };

  type t =
    | ConnectionParams(ConnectionParams.t)
    | Function(unit => ConnectionParams.t)
    | Promise(Js.Promise.t(ConnectionParams.t));

  let toJs: t => Js_.t =
    fun
    | ConnectionParams(v) => Js_.Union.connectionParams(v)
    | Function(v) => Js_.Union.fn(v)
    | Promise(v) => Js_.Union.promise(v);
};

module ClientOptions = {
  module Js_ = {
    // export interface ClientOptions {
    //     connectionParams?: ConnectionParamsOptions;
    //     timeout?: number;
    //     reconnect?: boolean;
    //     reconnectionAttempts?: number;
    //     connectionCallback?: (error: Error[], result?: any) => void;
    //     lazy?: boolean;
    //     inactivityTimeout?: number;
    // }
    type t = {
      connectionParams: option(ConnectionParamsOptions.Js_.t),
      timeout: option(int),
      reconnect: option(bool),
      reconnectionAttempts: option(int),
      connectionCallback:
        option(
          (~error: array(Js.Exn.t), ~result: option(Js.Json.t)) => unit,
        ),
      [@bs.as "lazy"]
      lazy_: option(bool),
      inactivityTimeout: option(int),
    };
  };

  type t = {
    connectionParams: option(ConnectionParamsOptions.t),
    timeout: option(int),
    reconnect: option(bool),
    reconnectionAttempts: option(int),
    connectionCallback:
      option((~error: array(Js.Exn.t), ~result: option(Js.Json.t)) => unit),
    [@bs.as "lazy"]
    lazy_: option(bool),
    inactivityTimeout: option(int),
  };

  let toJs: t => Js_.t =
    t => {
      connectionParams:
        t.connectionParams->Belt.Option.map(ConnectionParamsOptions.toJs),
      timeout: t.timeout,
      reconnect: t.reconnect,
      reconnectionAttempts: t.reconnectionAttempts,
      connectionCallback: t.connectionCallback,
      lazy_: t.lazy_,
      inactivityTimeout: t.inactivityTimeout,
    };

  let make =
      (
        ~connectionParams=?,
        ~timeout=?,
        ~reconnect=?,
        ~reconnectionAttempts=?,
        ~connectionCallback=?,
        ~lazy_=?,
        ~inactivityTimeout=?,
        (),
      )
      : t => {
    connectionParams,
    timeout,
    reconnect,
    reconnectionAttempts,
    connectionCallback,
    lazy_,
    inactivityTimeout,
  };
};

module SubscriptionClient = {
  module Js_ = {
    // export declare class SubscriptionClient {
    //     client: any;
    //     operations: Operations;
    //     constructor(url: string, options?: ClientOptions, webSocketImpl?: any, webSocketProtocols?: string | string[]);
    //     readonly status: any;
    //     close(isForced?: boolean, closedByUser?: boolean): void;
    //     request(request: OperationOptions): Observable<ExecutionResult>;
    //     on(eventName: string, callback: ListenerFn, context?: any): Function;
    //     onConnected(callback: ListenerFn, context?: any): Function;
    //     onConnecting(callback: ListenerFn, context?: any): Function;
    //     onDisconnected(callback: ListenerFn, context?: any): Function;
    //     onReconnected(callback: ListenerFn, context?: any): Function;
    //     onReconnecting(callback: ListenerFn, context?: any): Function;
    //     onError(callback: ListenerFn, context?: any): Function;
    //     unsubscribeAll(): void;
    //     applyMiddlewares(options: OperationOptions): Promise<OperationOptions>;
    //     use(middlewares: Middleware[]): SubscriptionClient;
    // }
    type t;
  };
  type t = Js_.t;
};
