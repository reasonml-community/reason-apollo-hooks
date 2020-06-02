module JsConsumer = {
  [@bs.module "react-apollo"] [@react.component]
  external make:
    (~children: ApolloClient_Client.t => React.element) => React.element =
    "ApolloConsumer";
};

[@react.component]
let make = (~children: ApolloClient_Client.t => React.element) =>
  <JsConsumer> {client => children(client)} </JsConsumer>;
