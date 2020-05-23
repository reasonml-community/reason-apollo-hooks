[@bs.module "@apollo/client"] [@react.component]
external make:
  (~client: ApolloClient_Client.t, ~children: React.element) => React.element =
  "ApolloProvider";
