module ApolloLink = ApolloClient__Link_Core_ApolloLink;
module ClientOptions = SubscriptionsTransportWs.ClientOptions;
module SubscriptionClient = SubscriptionsTransportWs.SubscriptionClient;

module WebSocketLink = {
  module Configuration = {
    module Js_ = {
      // export declare namespace WebSocketLink {
      //     interface Configuration {
      //         uri: string;
      //         options?: ClientOptions;
      //         webSocketImpl?: any;
      //     }
      // }
      type any;
      type t = {
        uri: string,
        options: option(ClientOptions.Js_.t),
        webSocketImpl: option(any),
      };
    };

    type t = Js_.t;
  };

  module Js_ = {
    // export declare class WebSocketLink extends ApolloLink {
    //     private subscriptionClient;
    //     constructor(paramsOrClient: WebSocketLink.Configuration | SubscriptionClient);
    //     request(operation: Operation): Observable<FetchResult> | null;
    // }
    [@bs.module "@apollo/link-ws"] [@bs.new]
    external make:
      (
      [@bs.unwrap]
      [
        | `Configuration(Configuration.Js_.t)
        | `SubscriptionClient(SubscriptionClient.Js_.t)
      ]
      ) =>
      ApolloLink.Js_.t =
      "WebSocketLink";
  };

  type webSocketImpl = Configuration.Js_.any;

  let make:
    (
      ~uri: string,
      ~options: ClientOptions.t=?,
      ~webSocketImpl: webSocketImpl=?,
      unit
    ) =>
    ApolloLink.t =
    (~uri, ~options=?, ~webSocketImpl=?, ()) => {
      Js_.make(
        `Configuration({
          uri,
          options: options->Belt.Option.map(ClientOptions.toJs),
          webSocketImpl,
        }),
      );
    };

  let make_withSubscriptionClient: SubscriptionClient.t => ApolloLink.t =
    subscriptionClient => {
      Js_.make(`SubscriptionClient(subscriptionClient));
    };
};
