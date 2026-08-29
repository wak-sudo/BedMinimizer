#include "CodeGen.h"

#include "DimacsWriter.h"
#include "Encoder.h"

Substitution CodeGen::genDimacsCodingToFile(std::shared_ptr<const IPreCalc> obj, unsigned long m, const std::string &filePath)
{
    const auto textToId = DimacsWriter::ToFile(filePath, Encoder(obj, m).genCoding());
    return Substitution(DimacsWriter::ReversMap(textToId));
}

Substitution CodeGen::genDimacsCodingToFileContains(std::shared_ptr<const IPreCalc> obj, unsigned long m, const std::string &filePath)
{
    const auto textToId = DimacsWriter::ToFile(filePath, Encoder(obj, m).genCodingTargetContains());
    return Substitution(DimacsWriter::ReversMap(textToId));
}

Substitution CodeGen::genDimacsCodingToFileContained(std::shared_ptr<const IPreCalc> obj, unsigned long m, const std::string &filePath)
{
    const auto textToId = DimacsWriter::ToFile(filePath, Encoder(obj, m).genCodingTargetContained());
    return Substitution(DimacsWriter::ReversMap(textToId));
}

std::pair<std::string, Substitution> CodeGen::genDimacsCodingToStr(std::shared_ptr<const IPreCalc> obj, unsigned long m)
{
    const auto [dimacCod, textToId] = DimacsWriter::ToString(Encoder(obj, m).genCoding());
    return make_pair(dimacCod, Substitution(DimacsWriter::ReversMap(textToId)));
}

