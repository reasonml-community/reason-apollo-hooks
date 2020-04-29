[@bs.module "@apollo/client"] [@react.component]
external make:
  (~client: ApolloClient.t, ~children: React.element) => React.element =
  "ApolloProvider";
