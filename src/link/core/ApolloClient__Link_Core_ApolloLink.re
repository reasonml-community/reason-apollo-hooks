module Operation = ApolloClient__Link_Core_Types.Operation;
module RequestHandler = ApolloClient__Link_Core_Types.RequestHandler;
// import { Observable } from '../../utilities/observables/Observable';
// import { NextLink, Operation, RequestHandler, FetchResult, GraphQLRequest } from './types';

module Js_ = {
  // export declare class ApolloLink {
  //     static empty(): ApolloLink;
  //     static from(links: (ApolloLink | RequestHandler)[]): ApolloLink;
  //     static split(test: (op: Operation) => boolean, left: ApolloLink | RequestHandler, right?: ApolloLink | RequestHandler): ApolloLink;
  //     static execute(link: ApolloLink, operation: GraphQLRequest): Observable<FetchResult>;
  //     static concat(first: ApolloLink | RequestHandler, second: ApolloLink | RequestHandler): ApolloLink;
  //     constructor(request?: RequestHandler);
  //     split(test: (op: Operation) => boolean, left: ApolloLink | RequestHandler, right?: ApolloLink | RequestHandler): ApolloLink;
  //     concat(next: ApolloLink | RequestHandler): ApolloLink;
  //     request(operation: Operation, forward?: NextLink): Observable<FetchResult> | null;
  //     protected onError(reason: any): void;
  //     setOnError(fn: (reason: any) => any): this;
  // }
  type t;

  [@bs.module "@apollo/client"] [@bs.new]
  external make: RequestHandler.Js_.t => t = "ApolloClient";

  [@bs.send] external concat: (t, t) => t = "concat";

  [@bs.send] external from: (t, array(t)) => t = "from";

  [@bs.send] external setOnError: (t, Js.Json.t => unit) => unit = "onError";

  [@bs.send]
  external split: (t, ~test: Operation.Js_.t => bool, ~left: t, ~right: t) => t =
    "split";
};

type t = Js_.t;

let make: RequestHandler.t => t = Js_.make;

let concat = Js_.concat;

let from = Js_.from;

let setOnError = Js_.setOnError;

let split = Js_.split;
