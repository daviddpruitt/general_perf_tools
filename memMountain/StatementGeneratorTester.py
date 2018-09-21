import StatementGenerator

MemOpGeneratorTestCases = [[{"opName":"tester"},["tester[  0]","tester[  1]","tester[  2]"]],
                           [{"opName":"input_"},["input_[  0]","input_[  1]","input_[  2]"]],
                           [{"opName":"inputArray","startIndex":0},["inputArray[  0]","inputArray[  1]","inputArray[  2]","inputArray[  3]","inputArray[  4]"]],
                           [{"opName":"inputArray","startIndex":2,"numIndex":4},["inputArray[  2]","inputArray[  3]","inputArray[  0]","inputArray[  1]","inputArray[  2]"]],
                           [{"opName":"inputArray","startIndex":2,"numIndex":8,"stride":3},["inputArray[  2]","inputArray[  5]","inputArray[  0]","inputArray[  3]","inputArray[  6]"]]]

RegOpGeneratorTestCases = [[{"opName":"tester"},["tester0","tester1","tester2"]],
                           [{"opName":"input_"},["input_0","input_1","input_2"]],
                           [{"opName":"inputArray","startIndex":0},["inputArray0","inputArray1","inputArray2","inputArray3","inputArray4"]],
                           [{"opName":"inputArray","startIndex":2,"numIndex":4},["inputArray2","inputArray3","inputArray0","inputArray1","inputArray2"]],
                           [{"opName":"inputArray","startIndex":2,"numIndex":4,"stride":2},["inputArray2","inputArray0","inputArray2","inputArray0","inputArray2"]]]



StmtMemOps = [[{"opName":"inputArray","startIndex":0,"numIndex":4,"stride":1},
               {"opName":"inputArray","startIndex":1,"numIndex":4,"stride":1},
               {"opName":"inputArray","startIndex":2,"numIndex":4,"stride":1}],
              [{"opName":"testArray", "startIndex":0,"numIndex":8,"stride":3},
               {"opName":"testArray", "startIndex":1,"numIndex":8,"stride":3},
               {"opName":"testArray", "startIndex":2,"numIndex":8,"stride":3}]]

StmtRegOps = [[{"opName":"inputData_","startIndex":0,"numIndex":4,"stride":1},
               {"opName":"inputData_","startIndex":1,"numIndex":4,"stride":1},
               {"opName":"inputData_","startIndex":2,"numIndex":4,"stride":1}],
              [{"opName":"testData_", "startIndex":0,"numIndex":8,"stride":3},
               {"opName":"testData_", "startIndex":1,"numIndex":8,"stride":3},
               {"opName":"testData_", "startIndex":2,"numIndex":8,"stride":3}],
              [{"opName":"testData_", "startIndex":0,"numIndex":8,"stride":3},
               {"opName":"testData_", "startIndex":1,"numIndex":8,"stride":-3},
               {"opName":"testData_", "startIndex":2,"numIndex":8,"stride":1}]]
StmntGenParams = [ {"firstOpType" :"mem","firstOpStartIdx" :0,"firstOpStride" :1,
                    "secondOpType":"reg","secondOpStartIdx":0,"secondOpStride":3,
                    "thirdOpType": "reg","thirdOpStartIdx" :2,"thirdOpStride" :3,
                    "regOpNumIndices":8,"memOpNumIndices":6,
                    "regOpName":"testData_","memOpName":"inputArray"},
                   {"firstOpType" :"reg","firstOpStartIdx" :0,"firstOpStride" :1,
                    "secondOpType":"mem","secondOpStartIdx":0,"secondOpStride":3,
                    "thirdOpType" :"mem","thirdOpStartIdx" :2,"thirdOpStride" :3,
                    "regOpNumIndices":8,"memOpNumIndices":6,
                    "regOpName":"testData_","memOpName":"inputArray"}]

FMAGeneratorTestCases = [[{"firstOpGenerator":StatementGenerator.MemOpGenerator(StmtMemOps[0][0]),
                           "secondOpGenerator":StatementGenerator.MemOpGenerator(StmtMemOps[0][1]),
                           "thirdOpGenerator":StatementGenerator.MemOpGenerator(StmtMemOps[0][2])},
                          ["inputArray[  0] = inputArray[  0] * inputArray[  1] + inputArray[  2];",
                           "inputArray[  1] = inputArray[  1] * inputArray[  2] + inputArray[  3];",
                           "inputArray[  2] = inputArray[  2] * inputArray[  3] + inputArray[  0];",
                           "inputArray[  3] = inputArray[  3] * inputArray[  0] + inputArray[  1];",
                           "inputArray[  0] = inputArray[  0] * inputArray[  1] + inputArray[  2];"]],
                           # Test memory operations on a non power of two stride
                         [{"firstOpGenerator":StatementGenerator.MemOpGenerator(StmtMemOps[1][0]),
                           "secondOpGenerator":StatementGenerator.MemOpGenerator(StmtMemOps[1][1]),
                           "thirdOpGenerator":StatementGenerator.MemOpGenerator(StmtMemOps[1][2])},
                          ["testArray[  0] = testArray[  0] * testArray[  1] + testArray[  2];",
                           "testArray[  3] = testArray[  3] * testArray[  4] + testArray[  5];",
                           "testArray[  6] = testArray[  6] * testArray[  7] + testArray[  0];",
                           "testArray[  1] = testArray[  1] * testArray[  2] + testArray[  3];",
                           "testArray[  4] = testArray[  4] * testArray[  5] + testArray[  6];"]],
                           # Test register operands as single strided
                         [{"firstOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[0][0]),
                           "secondOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[0][1]),
                           "thirdOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[0][2])},
                          ["inputData_0 = inputData_0 * inputData_1 + inputData_2;",
                           "inputData_1 = inputData_1 * inputData_2 + inputData_3;",
                           "inputData_2 = inputData_2 * inputData_3 + inputData_0;",
                           "inputData_3 = inputData_3 * inputData_0 + inputData_1;",
                           "inputData_0 = inputData_0 * inputData_1 + inputData_2;"]],
                           # Test register operands as a non power of two stride
                         [{"firstOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[1][0]),
                           "secondOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[1][1]),
                           "thirdOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[1][2])},
                          ["testData_0 = testData_0 * testData_1 + testData_2;",
                           "testData_3 = testData_3 * testData_4 + testData_5;",
                           "testData_6 = testData_6 * testData_7 + testData_0;",
                           "testData_1 = testData_1 * testData_2 + testData_3;",
                           "testData_4 = testData_4 * testData_5 + testData_6;"]],
                           # Test register operands with negative strides
                         [{"firstOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[2][0]),
                           "secondOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[2][1]),
                           "thirdOpGenerator":StatementGenerator.RegOpGenerator(StmtRegOps[2][2])},
                          ["testData_0 = testData_0 * testData_1 + testData_2;",
                           "testData_3 = testData_3 * testData_6 + testData_3;",
                           "testData_6 = testData_6 * testData_3 + testData_4;",
                           "testData_1 = testData_1 * testData_0 + testData_5;",
                           "testData_4 = testData_4 * testData_5 + testData_6;"]],
                           # Test FMA operands with mem destination
                         [StmntGenParams[0],
                          ["inputArray[  0] = inputArray[  0] * testData_0 + testData_2;",
                           "inputArray[  1] = inputArray[  1] * testData_3 + testData_5;",
                           "inputArray[  2] = inputArray[  2] * testData_6 + testData_0;",
                           "inputArray[  3] = inputArray[  3] * testData_1 + testData_3;",
                           "inputArray[  4] = inputArray[  4] * testData_4 + testData_6;"]],
                           # Test FMA operands with reg destination
                         [StmntGenParams[1],
                          ["testData_0 = testData_0 * inputArray[  0] + inputArray[  2];",
                           "testData_1 = testData_1 * inputArray[  3] + inputArray[  5];",
                           "testData_2 = testData_2 * inputArray[  0] + inputArray[  2];",
                           "testData_3 = testData_3 * inputArray[  3] + inputArray[  5];",
                           "testData_4 = testData_4 * inputArray[  0] + inputArray[  2];"]]]

MulGeneratorTestCases =  [# Test FMA operands with reg destination
                          [StmntGenParams[0],
                          ["inputArray[  0] = testData_0 * testData_2;",
                           "inputArray[  1] = testData_3 * testData_5;",
                           "inputArray[  2] = testData_6 * testData_0;",
                           "inputArray[  3] = testData_1 * testData_3;",
                           "inputArray[  4] = testData_4 * testData_6;"]],
                           # Test FMA operands with reg destination
                         [StmntGenParams[1],
                          ["testData_0 = inputArray[  0] * inputArray[  2];",
                           "testData_1 = inputArray[  3] * inputArray[  5];",
                           "testData_2 = inputArray[  0] * inputArray[  2];",
                           "testData_3 = inputArray[  3] * inputArray[  5];",
                           "testData_4 = inputArray[  0] * inputArray[  2];"]]]

AddGeneratorTestCases =  [# Test FMA operands with reg destination
                          [StmntGenParams[0],
                          ["inputArray[  0] = testData_0 + testData_2;",
                           "inputArray[  1] = testData_3 + testData_5;",
                           "inputArray[  2] = testData_6 + testData_0;",
                           "inputArray[  3] = testData_1 + testData_3;",
                           "inputArray[  4] = testData_4 + testData_6;"]],
                           # Test FMA operands with reg destination
                         [StmntGenParams[1],
                          ["testData_0 = inputArray[  0] + inputArray[  2];",
                           "testData_1 = inputArray[  3] + inputArray[  5];",
                           "testData_2 = inputArray[  0] + inputArray[  2];",
                           "testData_3 = inputArray[  3] + inputArray[  5];",
                           "testData_4 = inputArray[  0] + inputArray[  2];"]]]

PtrGeneratorTestCases =  [[{},
                          ["idx = inputArray[  idx];",
                           "idx = inputArray[  idx];",
                           "idx = inputArray[  idx];",
                           "idx = inputArray[  idx];",
                           "idx = inputArray[  idx];"]],
                           # Test FMA operands with reg destination
                         [{"arrayName":"dataArray", "indexName":"index"},
                          ["index = dataArray[  index];",
                           "index = dataArray[  index];",
                           "index = dataArray[  index];",
                           "index = dataArray[  index];",
                           "index = dataArray[  index];"]]]


class StatementGeneratorTester:
    """ Class that tests the StatementGenerator objects """
    def MemOpGeneratorTest(self,verbose=True):
        """ Tests the memory operand generator """
        for testCase in MemOpGeneratorTestCases:
            print("Testing memOpGenerator with params {0}".format(testCase[0]))
            memGenerator = StatementGenerator.MemOpGenerator(testCase[0])

            #Run through the test cases
            for result in testCase[1]:
                retVal = memGenerator.getNextOp(True)
                if verbose:
                    if retVal != result:
                        print("Failure")
                    else:
                        print("Success")
                    print("Expecting {0} got {1}".format(result, retVal))
                elif retVal != result:
                    print("Failure expected {0} got {1}".format(result, retVal))
                else:
                    print("Success")
            print("----------------------------------------------------")

    def RegOpGeneratorTest(self,verbose=False):
        """ Tests the memory operand generator """
        for testCase in RegOpGeneratorTestCases:
            print("Testing memOpGenerator with params {0}".format(testCase[0]))
            regGenerator = StatementGenerator.RegOpGenerator(testCase[0])

            #Run through the test cases
            for result in testCase[1]:
                retVal = regGenerator.getNextOp(True)
                if verbose:
                    if retVal != result:
                        print("Failure")
                    else:
                        print("Success")
                    print("Expecting {0} got {1}".format(result, retVal))
                elif retVal != result:
                    print("Failure expected {0} got {1}".format(result, retVal))
                else:
                    print("Success")
            print("----------------------------------------------------")

    def StatementTestCaseChecker(self, stmntGenerator, testCase, verbose):
        # Run through the test cases
        for result in testCase[1]:
            retVal = stmntGenerator.getNextStatement()
            if verbose:
                if retVal != result:
                    print("Failure")
                else:
                    print("Success")
                print("Expecting \n{0} got \n{1}".format(result, retVal))
            elif retVal != result:
                print("Failure expected \n{0} got \n{1}".format(result, retVal))
            else:
                print("Success")
        print("----------------------------------------------------")

    def FMAStatementGenerator(self,verbose=False):
        """ Tests the FMA statement generator """
        for testCase in FMAGeneratorTestCases:
            print("Testing FMA generator with params {0}".format(testCase[0]))
            stmntGenerator = StatementGenerator.FMAStatementGenerator(testCase[0])

            self.StatementTestCaseChecker(stmntGenerator, testCase, verbose)


    def MulStatementGenerator(self,verbose=False):
        """ Tests the Mul statement generator """
        for testCase in MulGeneratorTestCases:
            print("Testing Mul generator with params {0}".format(testCase[0]))
            stmntGenerator = StatementGenerator.MulStatementGenerator(testCase[0])

            self.StatementTestCaseChecker(stmntGenerator, testCase, verbose)


    def AddStatementGenerator(self,verbose=False):
        """ Tests the Add statement generator """
        for testCase in AddGeneratorTestCases:
            print("Testing Add generator with params {0}".format(testCase[0]))
            stmntGenerator = StatementGenerator.AddStatementGenerator(testCase[0])

            self.StatementTestCaseChecker(stmntGenerator, testCase, verbose)

    def PtrStatementGenerator(self,verbose=False):
        """ Tests the pointer chase statement generator """
        for testCase in PtrGeneratorTestCases:
            print("Testing pointer chase with params {0}".format(testCase[0]))
            stmntGenerator = StatementGenerator.PtrChaseStatementGenerator(testCase[0])

            self.StatementTestCaseChecker(stmntGenerator, testCase, verbose)


tester = StatementGeneratorTester()
tester.MemOpGeneratorTest(verbose=False)
tester.RegOpGeneratorTest(verbose=False)
tester.FMAStatementGenerator(verbose=False)
tester.MulStatementGenerator(verbose=False)
tester.AddStatementGenerator(verbose=False)
tester.PtrStatementGenerator(verbose=False)
