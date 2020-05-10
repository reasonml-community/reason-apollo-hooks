type parse('raw_t, 't) = 'raw_t => 't;
type serialize('t, 'raw_t) = 't => 'raw_t;
type query = string;

type graphqlDefinition('t, 'raw_t) = (
  parse('raw_t, 't),
  query,
  serialize('t, 'raw_t),
);
