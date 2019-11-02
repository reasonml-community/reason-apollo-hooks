[%bs.raw {|require('./styles.css')|}];

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=Client.instance>
    <ApolloHooks.ApolloProvider client=Client.instance>
      <> <EditPerson /> <Persons /> </>
    </ApolloHooks.ApolloProvider>
  </ReasonApollo.Provider>,
  "root",
);
