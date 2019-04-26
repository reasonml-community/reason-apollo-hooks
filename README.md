# reason-apollo-hooks

Reason bindings for https://github.com/trojanowski/react-apollo-hooks

## Installation

```
yarn add reason-apollo-hooks reason-apollo
```

Follow the installation instructions of [https://github.com/mhallin/graphql_ppx](graphql_ppx).

Then update your bsconfig.json

```diff
"bs-dependencies": [
  ...
+ "reason-apollo-hooks",
+ "reason-apollo"
]
```

# Available hooks

## useQuery

```reason
module UserQueryConfig = [%graphql {|
  query UserQuery {
    currentUser {
      name
    }
  }
|}];

module UserQuery = ReasonApolloHooks.Query.Make(UserQueryConfig);

[@react.component]
let make = () => {
  /* Both variant and records available */
  let ( result, { data, loading, error } ) = UserQuery.use();

  <div>
  {
    switch(result) {
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


## useMutation

```reason
module ScreamMutationConfig = [%graphql {|
  mutation ScreamMutation($screamLevel: Int!) {
    scream(level: $screamLevel) {
      error
    }
  }
|}];

module ScreamMutation = ReasonApolloHooks.Mutation.Make(ScreamMutationConfig);

[@react.component]
let make = () => {
  /* Both variant and records available */
  let screamMutation = ScreamMutation.use();
  let scream = (_) => {
    screamMutation(
      ScreamMutation.options(
        ~variables=ScreamMutationConfig.make(~screamLevel=10, ())##variables,
        ()
      ),
      ()
    )
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

## Getting it running

```sh
npm install
npm start
```
