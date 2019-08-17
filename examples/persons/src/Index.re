[%bs.raw {|require('./styles.css')|}];

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=Client.instance>
    <ReasonApolloHooks.ApolloProvider client=Client.instance>
      <> <EditPerson /> <Persons /> </>
    </ReasonApolloHooks.ApolloProvider>
  </ReasonApollo.Provider>,
  "root",
);
