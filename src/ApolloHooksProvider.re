[@bs.module "@apollo/client"] [@react.component]
external make:
  (
    ~client: ApolloClient.generatedApolloClient('raw_t),
    ~children: React.element
  ) =>
  React.element =
  "ApolloProvider";
