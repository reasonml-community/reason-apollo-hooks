[@bs.module "@apollo/client"] [@react.component]
external make:
  (~client: ApolloClient.generatedApolloClient, ~children: React.element) =>
  React.element =
  "ApolloProvider";
