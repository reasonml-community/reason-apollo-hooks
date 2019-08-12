ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=Client.instance>
    <ReasonApolloHooks.ApolloProvider client=Client.instance>
      <Persons />
    </ReasonApolloHooks.ApolloProvider>
  </ReasonApollo.Provider>,
  "root",
);
