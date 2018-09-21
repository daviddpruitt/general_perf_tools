import random

class OpGenerator:
    """ Abstract class that generates operators for statements """
    opName   = None
    index    = None
    maxIndex = None
    stride   = None
    def __init__(self, kwargs):
        """ Creates a new OpGenerator

        Keyword Arguments
        opName     -- the name/prefix of the operand
        startIndex -- the index to start at (default 0)
        numIndex   -- the number of indices available before the index has to overflow (default 256)
        stride     -- how much the index should be incremented (default 1)
        random     -- if indices should be randomized
        """

        self.opName   = kwargs.get('opName')
        self.index    = kwargs.get('startIndex', 0)
        self.numIndex = kwargs.get('numIndex'  , 256)
        self.stride   = kwargs.get('stride'    , 1)
        self.random   = kwargs.get('random'    , False)

    def incrementIndex(self):
        if self.random:
            self.index = random.randint(0, self.numIndex - 1)
        else:
            self.index = int((self.index + self.stride + self.numIndex) % self.numIndex)

    def getNextOp(self):
        pass

class MemOpGenerator(OpGenerator):
    """ Class that generates memory operators """
    def getNextOp(self, increment=False):
        retVal = self.opName + "[" + "{0:>3}".format(self.index) + "]"
        if increment:
            self.incrementIndex()
        return retVal

class RndMemOpGenerator(MemOpGenerator):
    """ Class that generate memory operators in random order """
    def incrementIndex(self):
        self.index = random.randint(0, self.numIndex - 1)

class RegOpGenerator(OpGenerator):
    """ Class that generates memory operators """
    def getNextOp(self, increment=False):
        retVal = self.opName + str(self.index)
        if increment:
            self.incrementIndex()
        return retVal

class RndRegOpGenerator(MemOpGenerator):
    """ Class that generate memory operators in random order """
    def incrementIndex(self):
        self.index = random.randint(0, self.numIndex - 1)


class StatementGenerator:
    """ Abstract class that generates C statements for benchmark apps """
    firstOpGenerator = None
    secondOpGenerator = None
    thirdOpGenerator = None
    arrayName = None
    def __init__(self, kwargs):
        #def __init__(self, firstOpType = "reg", secondOpType = "reg", thirdOptype = "reg", regOpName = "dataOp_", memOpName = "memOp",  firstOpGenerator = None, secondOpGenerator = None, thirdOpGenerator = None):
        """ Statement generator constructer, requires the op type and the names of the operands to use, conversely
            OpGenerator objects can be passed and will be used instead

            If the op generators aren't supplied then they will be created with
            the optional operand info

            Keyword Arguments
            firstOpGenerator  -- generator for the first operand
            secondOpGenerator -- generator for the secxond operand
            thirdOpGenerator  -- generator for the third operand

            firstOpType       -- First operand type  (default reg)
            firstOpStartIdx   -- First operand start index (default 0)
            firstOpStride     -- First operand stride (default 1)
            secondOpType      -- Second operand type (default reg)
            secondOpStartIdx  -- Second operand start index (default 1)
            secondOpStride    -- Second operand stride (default 1)
            thirdOptype       -- Third operand type  (default reg)
            thirdOpStartIdx   -- Third operand start index (default 2)
            thirdOpStride     -- Third operand stride (default 1)
            regOpNumIndices   -- Number of indices available for register ops (default 8)
            memOpNumIndices   -- Number of indices available for memory ops
            regOpName         -- Prefix for register operands (default "dataOp_")
            memOpName         -- Name for memory operands (defaul "memOp")

        """
        regOpNumIndices   = kwargs.get('regOpNumIndices',  8)
        memOpNumIndices   = kwargs.get('memOpNumIndices',  float('inf'))
        regOpName         = kwargs.get('regOpName',        'data_')
        memOpName         = kwargs.get('memOpName',        'memOp')

        if kwargs.get("firstOpGenerator") is None:
            firstOpType      = kwargs.get('firstOpType',      'reg')
            firstOpStartIdx  = kwargs.get('firstOpStartIdx',  0)
            firstOpStride    = kwargs.get('firstOpStride',    1)
            opts = {"startIndex":firstOpStartIdx,"stride":firstOpStride}

            if firstOpType.lower() == "mem":
                opts.update({"numIndex":memOpNumIndices,"opName":memOpName})
                self.firstOpGenerator = MemOpGenerator(opts)
            elif firstOpType.lower() == "reg":
                opts.update({"numIndex":regOpNumIndices,"opName":regOpName})
                self.firstOpGenerator = RegOpGenerator(opts)
            else:
                raise Exception ("firstOpType must be mem or reg, called with unrecognized option {0}".format(firstOpType))
        else:
            self.firstOpGenerator = kwargs.get("firstOpGenerator")

        if kwargs.get("secondOpGenerator") is None:
            secondOpType     = kwargs.get('secondOpType',     'reg')
            secondOpStartIdx = kwargs.get('secondOpStartIdx', 1)
            secondOpStride   = kwargs.get('secondOpStride',   1)
            opts = {"startIndex":secondOpStartIdx,"stride":secondOpStride}

            if secondOpType.lower() == "mem":
                opts.update({"numIndex":memOpNumIndices,"opName":memOpName})
                self.secondOpGenerator = MemOpGenerator(opts)
            elif secondOpType.lower() == "reg":
                opts.update({"numIndex":regOpNumIndices,"opName":regOpName})
                self.secondOpGenerator = RegOpGenerator(opts)
            else:
                raise Exception ("secondOpType must be mem or reg, called with unrecognized option {0}".format(secondOpType))
        else:
            self.secondOpGenerator = kwargs.get("secondOpGenerator")

        if kwargs.get("thirdOpGenerator") is None:
            thirdOpType      = kwargs.get('thirdOpType',      'reg')
            thirdOpStartIdx  = kwargs.get('thirdOpStartIdx',  2)
            thirdOpStride    = kwargs.get('thirdOpStride',    1)
            opts = {"startIndex":thirdOpStartIdx,"stride":thirdOpStride}

            if thirdOpType.lower() == "mem":
                opts.update({"numIndex":memOpNumIndices,"opName":memOpName})
                self.thirdOpGenerator = MemOpGenerator(opts)
            elif thirdOpType.lower() == "reg":
                opts.update({"numIndex":regOpNumIndices,"opName":regOpName})
                self.thirdOpGenerator = RegOpGenerator(opts)
            else:
                raise Exception ("thirdOpType must be mem or reg, called with unrecognized option {0}".format(thirdOpType))
        else:
            self.thirdOpGenerator = kwargs.get("thirdOpGenerator")

    def getNextStatement(self):
        """ generates the next statement in the sequence """
        pass

    def getStatements(self, kwargs):
        """ generates a set of statements in a sequence """
        numStmts    = kwargs.get('numStmts',     10)
        indentation = kwargs.get('indentation',  "\t\t")

        retVal = ""
        for stmnt in range(0, numStmnts):
            retVal = retVal + indentation + getNextStatement() + "\n"

class FMAStatementGenerator(StatementGenerator):
    """ Class that generates FMA statements for benchmark apps """

    def getNextStatement(self):
        retVal = self.firstOpGenerator.getNextOp(False) + " = " + \
                 self.firstOpGenerator.getNextOp(True)  + " * " + \
                 self.secondOpGenerator.getNextOp(True) + " + " + \
                 self.thirdOpGenerator.getNextOp(True)  + ";"
        return retVal

class MulStatementGenerator(StatementGenerator):
    """ Class that generates FMA statements for benchmark apps """

    def getNextStatement(self):
        retVal = self.firstOpGenerator.getNextOp(True) + " = " + \
                 self.secondOpGenerator.getNextOp(True)  + " * " + \
                 self.thirdOpGenerator.getNextOp(True) + ";"
        return retVal

class AddStatementGenerator(StatementGenerator):
    """ Class that generates FMA statements for benchmark apps """

    def getNextStatement(self):
        retVal = self.firstOpGenerator.getNextOp(True) + " = " + \
                 self.secondOpGenerator.getNextOp(True)  + " + " + \
                 self.thirdOpGenerator.getNextOp(True) + ";"
        return retVal

class PtrChaseStatementGenerator(StatementGenerator):
    """ Class that generates pointer chase statements for benchmark apps """
    arrayName = ""
    indexName = ""
    def __init__(self, kwargs):
        """ Pointer chase generator constructor

            Keyword Arguments
            arrayName  -- The name of the array
            indexName  -- The variable used to index the array
        """

        self.arrayName = kwargs.get("arrayName", "inputArray")
        self.indexName = kwargs.get("indexName", "idx")

    def getNextStatement(self):
        retVal = self.indexName + " = " + \
                 self.arrayName + "[  " + \
                 self.indexName + "];"
        return retVal
