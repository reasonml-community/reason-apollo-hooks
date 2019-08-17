# persons

## Run Persons Example

```sh
npm i
npm start
# in another tab
npm run server
```

When you make changes to the bindings in `reason-apollo-hooks`, the changes won't automatically propagate to the code in the example project, so you won't see any updates while it is running. This happens because there is still a previous local version of `reason-apollo-hooks` in `node_modules` (referenced as `"reason-apollo-hooks": "../../"` in `package.json`).

To get the updates, first build `reason-apollo-hooks`:

```sh
npm run clean
npm run buils
```

Then run `npm run update-deps`, which will remove the old versions of `reason-apollo-hooks` and get the latest local changes:

```sh
npm run update-deps
npm run clean
npm start
```
