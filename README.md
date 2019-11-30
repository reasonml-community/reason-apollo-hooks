# reason-apollo-hooks

Reason bindings for the official @apollo/react-hooks

## Installation

```
yarn add reason-apollo-hooks reason-apollo @apollo/react-hooks
```

Follow the installation instructions of [graphql_ppx_re](https://github.com/baransu/graphql_ppx_re).

Then update your bsconfig.json

```diff
"bs-dependencies": [
  ...
+ "reason-apollo-hooks",
+ "reason-apollo"
]
```

## Setting up

Add the provider in the top of the tree

```reason
/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="http://localhost:3010/graphql", ());

let client =
  ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());

let app =
 <ApolloHooks.Provider client>
   ...
 </ApolloHooks.Provider>
```

### Usage with reason-apollo

To use with `reason-apollo`'s `ReasonApollo.Provider` already present in your project:

```reason
let client = ... // create Apollo client

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client>
    <ApolloHooks.Provider client>
      <App />
    </ApolloHooks.Provider>
  </ReasonApollo.Provider>,
  "root",
);
```

# Available hooks

## useQuery

```reason
open ApolloHooks

module UserQuery = [%graphql {|
  query UserQuery {
    currentUser {
      name
    }
  }
|}];

[@react.component]
let make = () => {
  /* Both variant and records available */
  let (simple, _full) = useQuery(UserQuery.definition);

  <div>
  {
    switch(simple) {
      | Loading => <p>{React.string("Loading...")}</p>
      | Data(data) =>
        <p>{React.string(data##currentUser##name)}</p>
      | NoData
      | Error(_) => <p>{React.string("Get off my lawn!")}</p>
    }
  }
  </div>
}
```

Using the `full` record for more advanced cases

```reason
[@react.component]
let make = () => {
  /* Both variant and records available */
  let (_simple, full) = useQuery(UserQuery.definition);

  <div>
  {
    switch(full) {
      | { loading: true }=> <p>{React.string("Loading...")}</p>
      | { data: Some(data) } =>
        <p>{React.string(data##currentUser##name)}</p>
      | any other possibilities =>
      | { error: Some(_) } => <p>{React.string("Get off my lawn!")}</p>
    }
  }
  </div>
}
```

Using `fetchPolicy` to change interactions with the `apollo` cache, see [apollo docs](https://www.apollographql.com/docs/react/api/react-apollo/#optionsfetchpolicy).

```reason
let (_simple, full) = useQuery(~fetchPolicy=NetworkOnly, UserQuery.definition);
```

Using `errorPolicy` to change how errors are handled, see [apollo docs](https://www.apollographql.com/docs/react/api/react-apollo/#optionserrorpolicy).

```reason
let (simple, _full) = useQuery(~errorPolicy=All, UserQuery.definition);
```

Using `skip` to skip query entirely, see [apollo docs](https://www.apollographql.com/docs/react/api/react-apollo/#configskip).

```reason
let (simple, _full) =
  useQuery(
    ~skip=
      switch (value) {
      | None => true
      | _ => false
      },
    UserQuery.definition,
  );
```

## useMutation

```reason
module ScreamMutation = [%graphql {|
  mutation ScreamMutation($screamLevel: Int!) {
    scream(level: $screamLevel) {
      error
    }
  }
|}];

[@react.component]
let make = () => {
  /* Both variant and records available */
  let ( screamMutation, _simple, _full ) = useMutation(~variables=ScreamMutation.makeVariables(~screamLevel=10, ()), ScreamMutation.definition);
  let scream = (_) => {
    screamMutation()
      |> Js.Promise.then_(result => {
          switch(result) {
            | Data(data) => ...
            | Error(error) => ...
            | NoData => ...
          }
          Js.Promise.resolve()
        })
      |> ignore
  }

  <div>
    <button onClick={scream}>
      {React.string("You kids get off my lawn!")}
    </button>
  </div>
}
```

If you don't know the value of the variables yet you can pass them in later

```reason
[@react.component]
let make = () => {
  /* Both variant and records available */
  let ( screamMutation, _simple, _full ) = useMutation(ScreamMutation.definition);
  let scream = (_) => {
    screamMutation(~variables=ScreamMutation.makeVariables(~screamLevel=10, ()), ())
      |> Js.Promise.then_(result => {
          switch(result) {
            | Data(data) => ...
            | Error(error) => ...
            | NoData => ...
          }
          Js.Promise.resolve()
        })
      |> ignore
  }

  <div>
    <button onClick={scream}>
      {React.string("You kids get off my lawn!")}
    </button>
  </div>
}
```

## Cache

There are a couple of caveats with manual cache updates.

**TL;DR**

1. If you need to remove items from cached data, it is enough to just filter them out and save the result into cache as is.
2. If you need to add the result of a mutation to a list of items with the same shape, you simply concat it with the list and save into cache as it.
3. When you need to update a field, you have to resort to raw javascript to use spread operator on `Js.t` object in order to preserve `__typename` that `apollo` adds to all queries by default.

An example of cache update could look like this:

```reason
module PersonsQuery = [%graphql
{|
  query getAllPersons  {
    allPersons  {
      id
      age
      name
    }
  }
|}
];

module PersonsReadQuery = ApolloClient.ReadQuery(PersonsQuery);
module PersonsWriteQuery = ApolloClient.WriteQuery(PersonsQuery);

external cast: Js.Json.t => PersonsQuery.t = "%identity";

let updatePersons = (~client, ~name, ~age) => {
  let query = PersonsQuery.make();
  let readQueryOptions = ApolloHooks.Utils.toReadQueryOptions(query);

  // can throw exception of cache is empty
  switch (PersonsReadQuery.readQuery(client, readQueryOptions)) {
  | exception _ => ()
  | cachedResponse =>
    switch (cachedResponse |> Js.Nullable.toOption) {
    | None => ()
    | Some(cachedPersons) =>
      // readQuery will return unparsed data with __typename field, need to cast it since
      // it has type Json.t, but we know it will have the same type as PersonsReadQuery.t
      let persons = cast(cachedPersons);

      // to remove items, simply filter them out
      let updatedPersons = {
        "allPersons":
          Belt.Array.keep(persons##allPersons, person => person##age !== age),
      };

      // when updating items, __typename must be preserved, but since it is not a record,
      // can't use spread, so use JS to update items
      let updatedPersons = {
        "allPersons":
          Belt.Array.map(persons##allPersons, person =>
            person##name === name ? [%bs.raw {| {...person, age } |}] : person
          ),
      };

      PersonsWriteQuery.make(
        ~client,
        ~variables=query##variables,
        ~data=updatedPersons,
        (),
      );
    }
  };
};
```

`reason-apollo-hooks` parses response data from a query or mutation using parse function created by `graphql_ppx`. For example, when using `@bsRecord` directive, the response object will be parsed from a `Js.t` object to a reason record. In this case, the response data in reason code is not the same object that is stored in cache, since `react-apollo` saves data in cache before it is parsed and returned to the component. However, when updating cache, the data must be in the same format or apollo cache won't work correctly and throw errors.

If using directives like `@bsRecord`, `@bsDecoder` or `@bsVariant` in `graphql_ppx`, the data needs to be serialized back to JS object before it is written in cache. Since there is currently no way to serialize this data (see [this issue](https://github.com/mhallin/graphql_ppx/issues/71) in `graphql_ppx`), queries that will be updated in cache shouldn't use any of those directive, unless you will take care of the serialization yourself.

By default, apollo will add field `__typename` to the queries and will use it to normalize data and manipulate cache (see [normalization](https://www.apollographql.com/docs/react/advanced/caching/#normalization)). This field won't exist on parsed reason objects, since it is not included in the actual query you write, but is added by apollo before sending the query. Since `__typename` is crucial for the cache to work correctly, we need to read data from cache in its raw unparsed format, which is achieved with `readQuery` from `ApolloClient.ReadQuery` defined in `reason-apollo`.

## Getting it running

```sh
npm install
npm start
```
