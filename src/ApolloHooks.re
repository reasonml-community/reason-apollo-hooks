module Mutation = ApolloHooksMutation;
module Query = ApolloHooksQuery;
module Provider = ApolloHooksProvider;
module Subscription = ApolloHooksSubscription;

/**
  This is probably the one hook you'll use the most. A quick demo:

  {[
    open ApolloHooks;

    module Query = [%graphql {|
      query MyQuery {
        me { id, name }
      }
    |}];

    [@react.component]
    let make = () => {
      /* In Reason we prefix variables that we are not going to use with _ */
      let (simple, _full) = useQuery(Query.definitions);

      /* When using simple with Reason's pattern-matching operator, the compiler will force you to cover every single branch of the variant type */
      switch(simple) {
        | Loading => React.string("loading...")
        | Error(error) =>
          Js.log(error);
          React.string("Something went wrong!")
        | Data(data) =>
          React.string("Hello, " ++ data##me##name)
        /* Every. Single. One. Of Them. */
        | NoData =>
          React.string("Woa something went really wrong! Glady we use Reason and it forced us to handle this! Report this issue")
      }
    }
  ]}

  Why we return a tuple? While designing and using the API we came to the conclusion that would be much more convient to have a value that would attend
  the majority of simple usages and a full for when you need to do a complex UI, such as infinite scroll.

  The value [simple] ({!type:Query.variant('a)}) helps you to consume your data with simplicity, type safety and exhaustiveness check.
  But for those cases where you really want do do a fine-grained control of your data flow – such as when you have [loading] and [data] at the same time –
  that's when [full] ({!type:Query.queryResult('a)}) becomes more useful.

  {[
    module Query = [%graphql {|
      query MyQuery {
        me { id, name }
      }
    |}];

    [@react.component]
    let make = () => {
      let (_simple, full) = useQuery(Query.definitions);

      /* `full` is a record type so you pattern against it's possible combos of values */
      switch(full) {
        /* Initial loading */
        | { loading: true, data: None } => React.string("loading...")
        /* Error but no data */
        | { loading: false, data: None, error: Some(error) } => React.string("Something went wrong")
        /* When we have some data and we tried to refetch but got an error */
        | { loading: false, data: Some(data), error: Some(error)  } =>
          <>
            {React.string("Something went wrong")}
            <RenderData data onLoadMore={full.refetch} />
          </>
        /* Just data */
        | { loading: false, data: Some(data), error: None } =>
          <>
            {React.string("Something went wrong")}
            <RenderData data onLoadMore={full.refetch} />
          </>
        | Data(data) =>
          React.string("Hello, " ++ data##me##name)
        /* Not loading? No data? No error? That's weird */
        | {loading: false, data: None, error: null} =>
          React.string("Woa something went really wrong! But the programmer remembered to handle this case! Report to us")
      }
    }
  ]}

  Quite more complex right? Gladly it's not always that we have that level of complexity.

  That covers the most common cases of usage. If you want to see more complex usages check out the examples folder.
  */
let useQuery = Query.useQuery;

/**
  Second most used! Here's a quick demo:

  {[
    open ApolloHooks;

    module Mutation = [%graphql {|
      mutation MyMutation($input: MyMutationInput!) {
        myMutation(input: $input) { error }
      }
    |}];

    [@react.component]
    let make = () => {
      /* `simple` and `full` follow the same principle of `useQuery`. */
      let (mutate, simple, _full) = useMutation(Mutation.definitions);

      /* When using simple with Reason's pattern-matching operator, the compiler will force you to cover every single branch of the variant type */
      switch(simple) {
        | Loading => React.string("loading...")
        | Error(error) =>
          Js.log(error);
          React.string("Something went wrong!")
        | Data(data) =>
          <div>
            {React.string("Hello, " ++ data##me##name)}
          </div>
        /* Every. Single. One. Of Them. */
        | NotCalled => <button onClick={_ => mutate()}>{React.string("Click me")}
        | NoData =>
          React.string("Woa something went really wrong! Glady we use Reason and it forced us to handle this! Report this issue")
      }
    }
  ]}

  Or if you only care about calling [mutate]

  {[
    open ApolloHooks;

    module Mutation = [%graphql {|
      mutation MyMutation {
        me { id, name }
      }
    |}];

    [@react.component]
    let make = () => {
      let (mutate, _simple, _full) = useMutation(Mutation.definitions);
      let onClick = _event => {
        mutate()
          |> Js.Promise.then_(result => {
            switch(result) {
              | Data(data) => do anything here
              | Error(error) => handle your error
              | NoData => ...something went wrong...
            }
          })
      }

      <button onClick>{React.string("Click me")}</button>
    }
  ]}
  */
let useMutation = Mutation.useMutation;

/** useSubscription bindings */
let useSubscription = Subscription.useSubscription;

/** Helper to generate the shape of a query for [refetchQueries] mutation param. Take a look in examples/persons/src/EditPerson.re for a more complete demo of usage. */
let toQueryObj = result =>
  ApolloClient.{
    query: ApolloClient.gql(. result##query),
    variables: result##variables,
  };
