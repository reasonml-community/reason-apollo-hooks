NOTE: this is just documenting my process for this PR in case anyone wants to help. It's not necessarily intended to be used as the CONTRIBUTING file for `reason-apollo-hooks`

# Goals and Intent

My hope is that we can achieve well maintained, nearly-complete bindings without a huge burden on any single person if we follow these two rules:

1. Follow a consistent pattern for bindings
1. Avoid partial types or bindings if possible

## Going the Full Distance

Please type something as completely as possible when you come across it or leave it for someone else (if nothing else, put an abstract type so things will still flow through everywhere and people can cast it). If all of us contribute just a little piece, but do it completely, it should be very easy to get 99% complete bindings. Also, 50% of the work is in tracing through the code and loading up context. This way no one has to go back and duplicate that work. Each binding we add also makes the next one that much faster as we have more and more types we can reuse blindly.

# Guidelines (style)

## Directory Structure and File Naming

```
@apollo/client/react/hooks/useQuery.js
```

should become

```
reason-react-hooks/src/react/hooks/ApolloClient__React_Hooks_UseQuery.re
```

in reason.

### Breaking it down: `/[1]/[2]__[3]_[4]`

1. Reason files should be located in the same directory structure as the js counterpart (usually there is a `.d.ts` for every `.js` file so we can think of them interchangeably)
1. All module names should be prefixed with `ApolloClient__` "namespace"
1. File names reflect the directory structure
1. Files should be named the same as the js counterpart

## Types

- Every type goes in its own module with a `type t` (exception: see SubStypes)
- Every type module should contain a `type t`, a `Js_` module with a `type t`, and a `toJs` or `fromJs` conversion function. `t => Js_.t` or `Js_.t => t`
- Paste the type definition from the `.d.ts` file above the `type t` in the `Js_` module
- If data requires parsing/serializing create a new record even if the shape is the same. This ensures you don't forget to parse somewhere.
- Prefer single types when the more complicated type in a union can express both `shouldResubscribe?: boolean | ((options: BaseSubscriptionOptions<TData, TVariables>) => boolean);`
- Do all of this even if it doesn't need it (See Reasoning Behind `Js_` modules)

Example:

```
// #1 - create a Js_ module for your type
module Js_ = {
  // #2 - paste the definition from the .js file here
  // #3 - add the `.js` representation of type t = ...
}

// #4  - add the Reason representation of type t
// #4a - you can use `type t = Js_.t` if they are _exactly_ the same
// #4b - if they are the exact same _shape_ but requires parsing or serializing, *define a new record of the same shape* so we can leverage the fact that records are nominally typed to prevent someone forgetting to convert somewhere

// #5 - add `toJs` or `fromJs`. They often require parsing: `let fromJs: Js_.t('jsData, ~parse: 'jsData => 'data) => t('data)`
```

### Reasoning behind `Js_` modules

When I'm defining a `Js_` `type t` that references other types, I know I should always be using the `Js_.t` versions of those types and it's very easy to visually confirm the correct types are being referenced. The same goes for defining a top-level `type t`, I should never see a `Js_.t` there. Now I can just follow the compiler errors.

FWIW, I tried `JS` naming, but I would accidentally type `Js` all the time and not see it. Additionally, due to the importance of the above, I wanted it to stick out and the `_` helps a little there. :shrug:

### SubTypes

Sometimes multiple types were required to represent a single type in TypeScript. In order to help make it clear what is a binding to an actual type and what is just needed by Reason. In this case we take a similar naming approach to the modules (prefixing with the parent). For instance, `Apollo_Client__React_Types.QueryResult.Raw` has a `type t` that uses `t_fetchMoreOptions` which in turn uses `t_fetchMoreOptions_updateQueryOptions`.

## Binding to Js Module Exports

`externals` go under a `Js_` module and any types they reference should be `Js_.t` versions.

## Binding to Methods

Prefer T-first with `[@bs.send]`. Again, `externals` go under a `Js_` module with a `type t` and any types they reference should be `Js_.t` versions.

## Binding to Objects

Use records

## Binding to Enums

Use standard variants. You can use `jsConverter` for ints, but otherwise use manual `toJs` and `fromJs` functions.

## General

- Prefer T-first because that's the Reason community default
- Hooks are T-last because that makes sense given their usage
- ApolloClient methods are a Frankenstein T-first _and_ T-last because they want to maintain similarity with hooks api, but are also T-first due to [@bs.send] and T-first preference

# Exeptional Exception Handling

TBD
