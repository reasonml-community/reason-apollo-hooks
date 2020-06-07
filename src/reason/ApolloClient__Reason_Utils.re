let useGuaranteedMemo1 = (f, dependency) => {
  let value = React.useRef(f());
  let previousDependency = React.useRef(dependency);

  if (dependency !== previousDependency->React.Ref.current) {
    value->React.Ref.setCurrent(f());
    previousDependency->React.Ref.setCurrent(dependency);
  };

  value->React.Ref.current;
};
