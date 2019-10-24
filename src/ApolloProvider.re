[@bs.module "@apollo/react-hooks"] [@react.component]
external make:
  (~client: ApolloClient.generatedApolloClient, ~children: React.element) =>
  React.element =
  "ApolloProvider";
