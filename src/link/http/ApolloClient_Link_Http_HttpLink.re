module ApolloLink = ApolloClient__Link_Core_ApolloLink;
module HttpOptions = ApolloClient_Link_Http_SelectHttpOptionsAndBody.HttpOptions;

module Js_ = {
  // export declare class HttpLink extends ApolloLink {
  //     options: HttpOptions;
  //     requester: RequestHandler;
  //     constructor(options?: HttpOptions);
  // }
  [@bs.module "@apollo/client"] [@bs.new]
  external make: HttpOptions.Js_.t => ApolloLink.Js_.t = "HttpLink";
};

let make: HttpOptions.t => ApolloLink.t = Js_.make;
