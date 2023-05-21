

def zSql_flow_to(target: String) = {
  val src = cpg.call("sqlite3_exec").argument.order(2)
  val sink = cpg.call(target)
  sink.reachableByFlows(src).p
}

@main def exec(cpgFile: String, outFile: String, target: String) = {
  loadCpg(cpgFile)

  zSql_flow_to(target) |> outFile
}


