type parse('raw_t, 't) = 'raw_t => 't;
type serialize('t, 'raw_t) = 't => 'raw_t;
type query = string;

type graphqlDefinition('t, 'raw_t) = (
  parse('raw_t, 't),
  query,
  serialize('t, 'raw_t),
);

module type Operation = {
  let query: string;

  module Raw: {
    type t;
    type t_variables;
  };
  type t;

  let parse: Raw.t => t;
  let serialize: t => Raw.t;
};

module type OperationNoRequiredVars = {
  include Operation;
  let makeDefaultVariables: unit => Raw.t_variables;
};
