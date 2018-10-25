#include "math_toolbox.h"
#include <assert.h>
#include <string.h>

/* TODO: find a shorter way to initialize tree models
 * We create one model tree: each node keeps the label of the row it refers to
 * and the text which would be edited by clicking on the row. When the node is a
 * subtree, the edited text is set at I18n::Message::Default. */

const ToolboxNode calculChildren[4] = {ToolboxNode(I18n::Message::DiffCommandWithArg, I18n::Message::DerivateNumber, I18n::Message::DiffCommand), ToolboxNode(I18n::Message::IntCommandWithArg, I18n::Message::Integral, I18n::Message::IntCommand), ToolboxNode(I18n::Message::SumCommandWithArg, I18n::Message::Sum, I18n::Message::SumCommand), ToolboxNode(I18n::Message::ProductCommandWithArg, I18n::Message::Product, I18n::Message::ProductCommand)};
const ToolboxNode complexChildren[5] = {ToolboxNode(I18n::Message::AbsCommandWithArg, I18n::Message::ComplexAbsoluteValue, I18n::Message::AbsCommand), ToolboxNode(I18n::Message::ArgCommandWithArg, I18n::Message::Agument, I18n::Message::ArgCommand), ToolboxNode(I18n::Message::ReCommandWithArg, I18n::Message::ReelPart, I18n::Message::ReCommand), ToolboxNode(I18n::Message::ImCommandWithArg, I18n::Message::ImaginaryPart, I18n::Message::ImCommand), ToolboxNode(I18n::Message::ConjCommandWithArg, I18n::Message::Conjugate, I18n::Message::ConjCommand)};
const ToolboxNode probabilityChildren[2] = {ToolboxNode(I18n::Message::BinomialCommandWithArg, I18n::Message::Combination, I18n::Message::BinomialCommand), ToolboxNode(I18n::Message::PermuteCommandWithArg, I18n::Message::Permutation, I18n::Message::PermuteCommand)};
const ToolboxNode arithmeticChildren[4] = {ToolboxNode(I18n::Message::GcdCommandWithArg, I18n::Message::GreatCommonDivisor, I18n::Message::GcdCommand),ToolboxNode(I18n::Message::LcmCommandWithArg, I18n::Message::LeastCommonMultiple, I18n::Message::LcmCommand), ToolboxNode(I18n::Message::RemCommandWithArg, I18n::Message::Remainder, I18n::Message::RemCommand), ToolboxNode(I18n::Message::QuoCommandWithArg, I18n::Message::Quotient, I18n::Message::QuoCommand)};
#if MATRICES_ARE_DEFINED
const ToolboxNode matricesChildren[5] = {ToolboxNode(I18n::Message::InverseCommandWithArg, I18n::Message::Inverse, I18n::Message::InverseCommand), ToolboxNode(I18n::Message::DeterminantCommandWithArg, I18n::Message::Determinant, I18n::Message::DeterminantCommand), ToolboxNode(I18n::Message::TransposeCommandWithArg, I18n::Message::Transpose, I18n::Message::TransposeCommand), ToolboxNode(I18n::Message::TraceCommandWithArg, I18n::Message::Trace, I18n::Message::TraceCommand), ToolboxNode(I18n::Message::DimensionCommandWithArg, I18n::Message::Dimension, I18n::Message::DimensionCommand)};
#endif
#if LIST_ARE_DEFINED
const ToolboxNode listesChildren[5] = {ToolboxNode(I18n::Message::SortCommandWithArg, I18n::Message::Sort, I18n::Message::SortCommand), ToolboxNode(I18n::Message::InvSortCommandWithArg, I18n::Message::InvSort, I18n::Message::InvSortCommand), ToolboxNode(I18n::Message::MaxCommandWithArg, I18n::Message::Maximum, I18n::Message::MaxCommand), ToolboxNode(I18n::Message::MinCommandWithArg, I18n::Message::Minimum, I18n::Message::MinCommand), ToolboxNode(I18n::Message::DimensionCommandWithArg, I18n::Message::Dimension, I18n::Message::DimensionCommand)};
#endif
const ToolboxNode approximationChildren[4] = {ToolboxNode(I18n::Message::FloorCommandWithArg, I18n::Message::Floor, I18n::Message::FloorCommand), ToolboxNode(I18n::Message::FracCommandWithArg, I18n::Message::FracPart, I18n::Message::FracCommand), ToolboxNode(I18n::Message::CeilCommandWithArg, I18n::Message::Ceiling, I18n::Message::CeilCommand), ToolboxNode(I18n::Message::RoundCommandWithArg, I18n::Message::Rounding, I18n::Message::RoundCommand)};
const ToolboxNode trigonometryChildren[6] = {ToolboxNode(I18n::Message::CoshCommandWithArg, I18n::Message::HyperbolicCosine, I18n::Message::CoshCommand), ToolboxNode(I18n::Message::SinhCommandWithArg, I18n::Message::HyperbolicSine, I18n::Message::SinhCommand), ToolboxNode(I18n::Message::TanhCommandWithArg, I18n::Message::HyperbolicTangent, I18n::Message::TanhCommand), ToolboxNode(I18n::Message::AcoshCommandWithArg, I18n::Message::InverseHyperbolicCosine, I18n::Message::AcoshCommand), ToolboxNode(I18n::Message::AsinhCommandWithArg, I18n::Message::InverseHyperbolicSine, I18n::Message::AsinhCommand), ToolboxNode(I18n::Message::AtanhCommandWithArg, I18n::Message::InverseHyperbolicTangent, I18n::Message::AtanhCommand)};
const ToolboxNode predictionChildren[3] = {ToolboxNode(I18n::Message::Prediction95CommandWithArg, I18n::Message::Prediction95, I18n::Message::Prediction95Command), ToolboxNode(I18n::Message::PredictionCommandWithArg, I18n::Message::Prediction, I18n::Message::PredictionCommand), ToolboxNode(I18n::Message::ConfidenceCommandWithArg, I18n::Message::Confidence, I18n::Message::ConfidenceCommand)};

// BEGIN CHEMISTRY CONSTANTS
const ToolboxNode periodicTableChildren[119] = {
  ToolboxNode(I18n::Message::HydrogenName, I18n::Message::HydrogenMolarMass, I18n::Message::HydrogenMolarMass),
  ToolboxNode(I18n::Message::HeliumName, I18n::Message::HeliumMolarMass, I18n::Message::HeliumMolarMass),
  ToolboxNode(I18n::Message::LithiumName, I18n::Message::LithiumMolarMass, I18n::Message::LithiumMolarMass),
  ToolboxNode(I18n::Message::BerylliumName, I18n::Message::BerylliumMolarMass, I18n::Message::BerylliumMolarMass),
  ToolboxNode(I18n::Message::BoronName, I18n::Message::BoronMolarMass, I18n::Message::BoronMolarMass),
  ToolboxNode(I18n::Message::CarbonName, I18n::Message::CarbonMolarMass, I18n::Message::CarbonMolarMass),
  ToolboxNode(I18n::Message::NitrogenName, I18n::Message::NitrogenMolarMass, I18n::Message::NitrogenMolarMass),
  ToolboxNode(I18n::Message::OxygenName, I18n::Message::OxygenMolarMass, I18n::Message::OxygenMolarMass),
  ToolboxNode(I18n::Message::FluorineName, I18n::Message::FluorineMolarMass, I18n::Message::FluorineMolarMass),
  ToolboxNode(I18n::Message::NeonName, I18n::Message::NeonMolarMass, I18n::Message::NeonMolarMass),
  ToolboxNode(I18n::Message::SodiumName, I18n::Message::SodiumMolarMass, I18n::Message::SodiumMolarMass),
  ToolboxNode(I18n::Message::MagnesiumName, I18n::Message::MagnesiumMolarMass, I18n::Message::MagnesiumMolarMass),
  ToolboxNode(I18n::Message::AluminiumName, I18n::Message::AluminiumMolarMass, I18n::Message::AluminiumMolarMass),
  ToolboxNode(I18n::Message::SiliconName, I18n::Message::SiliconMolarMass, I18n::Message::SiliconMolarMass),
  ToolboxNode(I18n::Message::PhosphorusName, I18n::Message::PhosphorusMolarMass, I18n::Message::PhosphorusMolarMass),
  ToolboxNode(I18n::Message::SulfurName, I18n::Message::SulfurMolarMass, I18n::Message::SulfurMolarMass),
  ToolboxNode(I18n::Message::ChlorineName, I18n::Message::ChlorineMolarMass, I18n::Message::ChlorineMolarMass),
  ToolboxNode(I18n::Message::ArgonName, I18n::Message::ArgonMolarMass, I18n::Message::ArgonMolarMass),
  ToolboxNode(I18n::Message::PotassiumName, I18n::Message::PotassiumMolarMass, I18n::Message::PotassiumMolarMass),
  ToolboxNode(I18n::Message::CalciumName, I18n::Message::CalciumMolarMass, I18n::Message::CalciumMolarMass),
  ToolboxNode(I18n::Message::ScandiumName, I18n::Message::ScandiumMolarMass, I18n::Message::ScandiumMolarMass),
  ToolboxNode(I18n::Message::TitaniumName, I18n::Message::TitaniumMolarMass, I18n::Message::TitaniumMolarMass),
  ToolboxNode(I18n::Message::VanadiumName, I18n::Message::VanadiumMolarMass, I18n::Message::VanadiumMolarMass),
  ToolboxNode(I18n::Message::ChromiumName, I18n::Message::ChromiumMolarMass, I18n::Message::ChromiumMolarMass),
  ToolboxNode(I18n::Message::ManganeseName, I18n::Message::ManganeseMolarMass, I18n::Message::ManganeseMolarMass),
  ToolboxNode(I18n::Message::IronName, I18n::Message::IronMolarMass, I18n::Message::IronMolarMass),
  ToolboxNode(I18n::Message::CobaltName, I18n::Message::CobaltMolarMass, I18n::Message::CobaltMolarMass),
  ToolboxNode(I18n::Message::NickelName, I18n::Message::NickelMolarMass, I18n::Message::NickelMolarMass),
  ToolboxNode(I18n::Message::CopperName, I18n::Message::CopperMolarMass, I18n::Message::CopperMolarMass),
  ToolboxNode(I18n::Message::ZincName, I18n::Message::ZincMolarMass, I18n::Message::ZincMolarMass),
  ToolboxNode(I18n::Message::GalliumName, I18n::Message::GalliumMolarMass, I18n::Message::GalliumMolarMass),
  ToolboxNode(I18n::Message::GermaniumName, I18n::Message::GermaniumMolarMass, I18n::Message::GermaniumMolarMass),
  ToolboxNode(I18n::Message::ArsenicName, I18n::Message::ArsenicMolarMass, I18n::Message::ArsenicMolarMass),
  ToolboxNode(I18n::Message::SeleniumName, I18n::Message::SeleniumMolarMass, I18n::Message::SeleniumMolarMass),
  ToolboxNode(I18n::Message::BromineName, I18n::Message::BromineMolarMass, I18n::Message::BromineMolarMass),
  ToolboxNode(I18n::Message::KryptonName, I18n::Message::KryptonMolarMass, I18n::Message::KryptonMolarMass),
  ToolboxNode(I18n::Message::RubidiumName, I18n::Message::RubidiumMolarMass, I18n::Message::RubidiumMolarMass),
  ToolboxNode(I18n::Message::StrontiumName, I18n::Message::StrontiumMolarMass, I18n::Message::StrontiumMolarMass),
  ToolboxNode(I18n::Message::YttriumName, I18n::Message::YttriumMolarMass, I18n::Message::YttriumMolarMass),
  ToolboxNode(I18n::Message::ZirconiumName, I18n::Message::ZirconiumMolarMass, I18n::Message::ZirconiumMolarMass),
  ToolboxNode(I18n::Message::NiobiumName, I18n::Message::NiobiumMolarMass, I18n::Message::NiobiumMolarMass),
  ToolboxNode(I18n::Message::MolybdenumName, I18n::Message::MolybdenumMolarMass, I18n::Message::MolybdenumMolarMass),
  ToolboxNode(I18n::Message::TechnetiumName, I18n::Message::TechnetiumMolarMass, I18n::Message::TechnetiumMolarMass),
  ToolboxNode(I18n::Message::RutheniumName, I18n::Message::RutheniumMolarMass, I18n::Message::RutheniumMolarMass),
  ToolboxNode(I18n::Message::RhodiumName, I18n::Message::RhodiumMolarMass, I18n::Message::RhodiumMolarMass),
  ToolboxNode(I18n::Message::PalladiumName, I18n::Message::PalladiumMolarMass, I18n::Message::PalladiumMolarMass),
  ToolboxNode(I18n::Message::SilverName, I18n::Message::SilverMolarMass, I18n::Message::SilverMolarMass),
  ToolboxNode(I18n::Message::CadmiumName, I18n::Message::CadmiumMolarMass, I18n::Message::CadmiumMolarMass),
  ToolboxNode(I18n::Message::IndiumName, I18n::Message::IndiumMolarMass, I18n::Message::IndiumMolarMass),
  ToolboxNode(I18n::Message::TinName, I18n::Message::TinMolarMass, I18n::Message::TinMolarMass),
  ToolboxNode(I18n::Message::AntimonyName, I18n::Message::AntimonyMolarMass, I18n::Message::AntimonyMolarMass),
  ToolboxNode(I18n::Message::TelluriumName, I18n::Message::TelluriumMolarMass, I18n::Message::TelluriumMolarMass),
  ToolboxNode(I18n::Message::IodineName, I18n::Message::IodineMolarMass, I18n::Message::IodineMolarMass),
  ToolboxNode(I18n::Message::XenonName, I18n::Message::XenonMolarMass, I18n::Message::XenonMolarMass),
  ToolboxNode(I18n::Message::CesiumName, I18n::Message::CesiumMolarMass, I18n::Message::CesiumMolarMass),
  ToolboxNode(I18n::Message::BariumName, I18n::Message::BariumMolarMass, I18n::Message::BariumMolarMass),
  ToolboxNode(I18n::Message::LanthanumName, I18n::Message::LanthanumMolarMass, I18n::Message::LanthanumMolarMass),
  ToolboxNode(I18n::Message::CeriumName, I18n::Message::CeriumMolarMass, I18n::Message::CeriumMolarMass),
  ToolboxNode(I18n::Message::PraseodymiumName, I18n::Message::PraseodymiumMolarMass, I18n::Message::PraseodymiumMolarMass),
  ToolboxNode(I18n::Message::NeodymiumName, I18n::Message::NeodymiumMolarMass, I18n::Message::NeodymiumMolarMass),
  ToolboxNode(I18n::Message::PromethiumName, I18n::Message::PromethiumMolarMass, I18n::Message::PromethiumMolarMass),
  ToolboxNode(I18n::Message::SamariumName, I18n::Message::SamariumMolarMass, I18n::Message::SamariumMolarMass),
  ToolboxNode(I18n::Message::EuropiumName, I18n::Message::EuropiumMolarMass, I18n::Message::EuropiumMolarMass),
  ToolboxNode(I18n::Message::GadoliniumName, I18n::Message::GadoliniumMolarMass, I18n::Message::GadoliniumMolarMass),
  ToolboxNode(I18n::Message::TerbiumName, I18n::Message::TerbiumMolarMass, I18n::Message::TerbiumMolarMass),
  ToolboxNode(I18n::Message::DysprosiumName, I18n::Message::DysprosiumMolarMass, I18n::Message::DysprosiumMolarMass),
  ToolboxNode(I18n::Message::HolmiumName, I18n::Message::HolmiumMolarMass, I18n::Message::HolmiumMolarMass),
  ToolboxNode(I18n::Message::ErbiumName, I18n::Message::ErbiumMolarMass, I18n::Message::ErbiumMolarMass),
  ToolboxNode(I18n::Message::ThuliumName, I18n::Message::ThuliumMolarMass, I18n::Message::ThuliumMolarMass),
  ToolboxNode(I18n::Message::YtterbiumName, I18n::Message::YtterbiumMolarMass, I18n::Message::YtterbiumMolarMass),
  ToolboxNode(I18n::Message::LutetiumName, I18n::Message::LutetiumMolarMass, I18n::Message::LutetiumMolarMass),
  ToolboxNode(I18n::Message::HafniumName, I18n::Message::HafniumMolarMass, I18n::Message::HafniumMolarMass),
  ToolboxNode(I18n::Message::TantalumName, I18n::Message::TantalumMolarMass, I18n::Message::TantalumMolarMass),
  ToolboxNode(I18n::Message::TungstenName, I18n::Message::TungstenMolarMass, I18n::Message::TungstenMolarMass),
  ToolboxNode(I18n::Message::RheniumName, I18n::Message::RheniumMolarMass, I18n::Message::RheniumMolarMass),
  ToolboxNode(I18n::Message::OsmiumName, I18n::Message::OsmiumMolarMass, I18n::Message::OsmiumMolarMass),
  ToolboxNode(I18n::Message::IridiumName, I18n::Message::IridiumMolarMass, I18n::Message::IridiumMolarMass),
  ToolboxNode(I18n::Message::PlatinumName, I18n::Message::PlatinumMolarMass, I18n::Message::PlatinumMolarMass),
  ToolboxNode(I18n::Message::GoldName, I18n::Message::GoldMolarMass, I18n::Message::GoldMolarMass),
  ToolboxNode(I18n::Message::MercuryName, I18n::Message::MercuryMolarMass, I18n::Message::MercuryMolarMass),
  ToolboxNode(I18n::Message::ThalliumName, I18n::Message::ThalliumMolarMass, I18n::Message::ThalliumMolarMass),
  ToolboxNode(I18n::Message::LeadName, I18n::Message::LeadMolarMass, I18n::Message::LeadMolarMass),
  ToolboxNode(I18n::Message::BismuthName, I18n::Message::BismuthMolarMass, I18n::Message::BismuthMolarMass),
  ToolboxNode(I18n::Message::PoloniumName, I18n::Message::PoloniumMolarMass, I18n::Message::PoloniumMolarMass),
  ToolboxNode(I18n::Message::AstatineName, I18n::Message::AstatineMolarMass, I18n::Message::AstatineMolarMass),
  ToolboxNode(I18n::Message::RadonName, I18n::Message::RadonMolarMass, I18n::Message::RadonMolarMass),
  ToolboxNode(I18n::Message::FranciumName, I18n::Message::FranciumMolarMass, I18n::Message::FranciumMolarMass),
  ToolboxNode(I18n::Message::RadiumName, I18n::Message::RadiumMolarMass, I18n::Message::RadiumMolarMass),
  ToolboxNode(I18n::Message::ActiniumName, I18n::Message::ActiniumMolarMass, I18n::Message::ActiniumMolarMass),
  ToolboxNode(I18n::Message::ThoriumName, I18n::Message::ThoriumMolarMass, I18n::Message::ThoriumMolarMass),
  ToolboxNode(I18n::Message::ProtactiniumName, I18n::Message::ProtactiniumMolarMass, I18n::Message::ProtactiniumMolarMass),
  ToolboxNode(I18n::Message::UraniumName, I18n::Message::UraniumMolarMass, I18n::Message::UraniumMolarMass),
  ToolboxNode(I18n::Message::NeptuniumName, I18n::Message::NeptuniumMolarMass, I18n::Message::NeptuniumMolarMass),
  ToolboxNode(I18n::Message::PlutoniumName, I18n::Message::PlutoniumMolarMass, I18n::Message::PlutoniumMolarMass),
  ToolboxNode(I18n::Message::AmericiumName, I18n::Message::AmericiumMolarMass, I18n::Message::AmericiumMolarMass),
  ToolboxNode(I18n::Message::CuriumName, I18n::Message::CuriumMolarMass, I18n::Message::CuriumMolarMass),
  ToolboxNode(I18n::Message::BerkeliumName, I18n::Message::BerkeliumMolarMass, I18n::Message::BerkeliumMolarMass),
  ToolboxNode(I18n::Message::CaliforniumName, I18n::Message::CaliforniumMolarMass, I18n::Message::CaliforniumMolarMass),
  ToolboxNode(I18n::Message::EinsteiniumName, I18n::Message::EinsteiniumMolarMass, I18n::Message::EinsteiniumMolarMass),
  ToolboxNode(I18n::Message::FermiumName, I18n::Message::FermiumMolarMass, I18n::Message::FermiumMolarMass),
  ToolboxNode(I18n::Message::MendeleviumName, I18n::Message::MendeleviumMolarMass, I18n::Message::MendeleviumMolarMass),
  ToolboxNode(I18n::Message::NobeliumName, I18n::Message::NobeliumMolarMass, I18n::Message::NobeliumMolarMass),
  ToolboxNode(I18n::Message::LawrenciumName, I18n::Message::LawrenciumMolarMass, I18n::Message::LawrenciumMolarMass),
  ToolboxNode(I18n::Message::RutherfordiumName, I18n::Message::RutherfordiumMolarMass, I18n::Message::RutherfordiumMolarMass),
  ToolboxNode(I18n::Message::DubniumName, I18n::Message::DubniumMolarMass, I18n::Message::DubniumMolarMass),
  ToolboxNode(I18n::Message::SeaborgiumName, I18n::Message::SeaborgiumMolarMass, I18n::Message::SeaborgiumMolarMass),
  ToolboxNode(I18n::Message::BohriumName, I18n::Message::BohriumMolarMass, I18n::Message::BohriumMolarMass),
  ToolboxNode(I18n::Message::HassiumName, I18n::Message::HassiumMolarMass, I18n::Message::HassiumMolarMass),
  ToolboxNode(I18n::Message::MeitneriumName, I18n::Message::MeitneriumMolarMass, I18n::Message::MeitneriumMolarMass),
  ToolboxNode(I18n::Message::DarmstadtiumName, I18n::Message::DarmstadtiumMolarMass, I18n::Message::DarmstadtiumMolarMass),
  ToolboxNode(I18n::Message::RoentgeniumName, I18n::Message::RoentgeniumMolarMass, I18n::Message::RoentgeniumMolarMass),
  ToolboxNode(I18n::Message::CoperniciumName, I18n::Message::CoperniciumMolarMass, I18n::Message::CoperniciumMolarMass),
  ToolboxNode(I18n::Message::NihoniumName, I18n::Message::NihoniumMolarMass, I18n::Message::NihoniumMolarMass),
  ToolboxNode(I18n::Message::FleroviumName, I18n::Message::FleroviumMolarMass, I18n::Message::FleroviumMolarMass),
  ToolboxNode(I18n::Message::MoscoviumName, I18n::Message::MoscoviumMolarMass, I18n::Message::MoscoviumMolarMass),
  ToolboxNode(I18n::Message::LivermoriumName, I18n::Message::LivermoriumMolarMass, I18n::Message::LivermoriumMolarMass),
  ToolboxNode(I18n::Message::TennessineName, I18n::Message::TennessineMolarMass, I18n::Message::TennessineMolarMass),
  ToolboxNode(I18n::Message::OganessonName, I18n::Message::OganessonMolarMass, I18n::Message::OganessonMolarMass),
  ToolboxNode(I18n::Message::ChemistryDataCredit, I18n::Message::Default, I18n::Message::Default),
};

const ToolboxNode chemistryChildren[2] = {
  ToolboxNode(I18n::Message::AvogadrosNumber, I18n::Message::AvogadrosNumberValue, I18n::Message::AvogadrosNumberValue),
  ToolboxNode(I18n::Message::MolarMasses, I18n::Message::Default, I18n::Message::Default, periodicTableChildren, 119),
};

#if LIST_ARE_DEFINED
const ToolboxNode menu[13] = {ToolboxNode(I18n::Message::AbsCommandWithArg, I18n::Message::AbsoluteValue, I18n::Message::AbsCommand),
#elif MATRICES_ARE_DEFINED
const ToolboxNode menu[12] = {ToolboxNode(I18n::Message::AbsCommandWithArg, I18n::Message::AbsoluteValue, I18n::Message::AbsCommand),
#else
const ToolboxNode menu[11] = {ToolboxNode(I18n::Message::AbsCommandWithArg, I18n::Message::AbsoluteValue, I18n::Message::AbsCommand),
#endif
  ToolboxNode(I18n::Message::RootCommandWithArg, I18n::Message::NthRoot, I18n::Message::RootCommand),
  ToolboxNode(I18n::Message::LogCommandWithArg, I18n::Message::BasedLogarithm, I18n::Message::LogCommand),
  ToolboxNode(I18n::Message::Calculation, I18n::Message::Default, I18n::Message::Default, calculChildren, 4),
  ToolboxNode(I18n::Message::ComplexNumber, I18n::Message::Default, I18n::Message::Default, complexChildren, 5),
  ToolboxNode(I18n::Message::Probability, I18n::Message::Default, I18n::Message::Default, probabilityChildren, 2),
  ToolboxNode(I18n::Message::Arithmetic, I18n::Message::Default, I18n::Message::Default, arithmeticChildren, 4),
#if MATRICES_ARE_DEFINED
  ToolboxNode(I18n::Message::Matrices,  I18n::Message::Default, I18n::Message::Default, matricesChildren, 5),
#endif
#if LIST_ARE_DEFINED
  ToolboxNode(I18n::Message::Lists, I18n::Message::Default, I18n::Message::Default, listesChildren, 5),
#endif
  ToolboxNode(I18n::Message::Approximation, I18n::Message::Default, I18n::Message::Default, approximationChildren, 4),
  ToolboxNode(I18n::Message::HyperbolicTrigonometry, I18n::Message::Default, I18n::Message::Default, trigonometryChildren, 6),
  ToolboxNode(I18n::Message::Fluctuation, I18n::Message::Default, I18n::Message::Default, predictionChildren, 3),
  ToolboxNode(I18n::Message::Chemistry, I18n::Message::Default, I18n::Message::Default, chemistryChildren, 2)};
#if LIST_ARE_DEFINED
const ToolboxNode toolboxModel = ToolboxNode(I18n::Message::Toolbox, I18n::Message::Default, I18n::Message::Default, menu, 13);
#elif MATRICES_ARE_DEFINED
const ToolboxNode toolboxModel = ToolboxNode(I18n::Message::Toolbox, I18n::Message::Default, I18n::Message::Default, menu, 12);
#else
const ToolboxNode toolboxModel = ToolboxNode(I18n::Message::Toolbox, I18n::Message::Default, I18n::Message::Default, menu, 11);
#endif

/* State */

MathToolbox::Stack::State::State(int selectedRow, KDCoordinate verticalScroll) :
  m_selectedRow(selectedRow),
  m_verticalScroll(verticalScroll)
{
}

int MathToolbox::Stack::State::selectedRow() {
  return m_selectedRow;
}

KDCoordinate MathToolbox::Stack::State::verticalScroll() {
  return m_verticalScroll;
}

bool MathToolbox::Stack::State::isNull(){
  if (m_selectedRow == -1) {
    return true;
  }
  return false;
}

/* Stack */

void MathToolbox::Stack::push(int selectedRow, KDCoordinate verticalScroll) {
  int i = 0;
  while (!m_statesStack[i].isNull() && i < k_maxModelTreeDepth) {
    i++;
  }
  assert(m_statesStack[i].isNull());
  m_statesStack[i] = State(selectedRow, verticalScroll);
}

MathToolbox::Stack::State * MathToolbox::Stack::stateAtIndex(int index) {
  return &m_statesStack[index];
}

int MathToolbox::Stack::depth() {
  int depth = 0;
  for (int i = 0; i < k_maxModelTreeDepth; i++) {
    depth += (!m_statesStack[i].isNull());
  }
  return depth;
}

void MathToolbox::Stack::pop() {
  int stackDepth = depth();
  if (stackDepth == 0) {
    return;
  }
  m_statesStack[stackDepth-1] = State();
}

void MathToolbox::Stack::resetStack() {
  for (int i = 0; i < k_maxModelTreeDepth; i++) {
    m_statesStack[i] = State();
  }
}

/* List Controller */

MathToolbox::ListController::ListController(Responder * parentResponder, SelectableTableView * tableView) :
  ViewController(parentResponder),
  m_selectableTableView(tableView),
  m_firstSelectedRow(0)
{
}

View * MathToolbox::ListController::view() {
  return m_selectableTableView;
}

const char * MathToolbox::ListController::title() {
  return I18n::translate(toolboxModel.label());
}

void MathToolbox::ListController::didBecomeFirstResponder() {
  m_selectableTableView->reloadData();
  m_selectableTableView->selectCellAtLocation(0, m_firstSelectedRow);
  app()->setFirstResponder(m_selectableTableView);
}

void MathToolbox::ListController::setFirstSelectedRow(int firstSelectedRow) {
  m_firstSelectedRow = firstSelectedRow;
}

/* MathToolbox */

MathToolbox::MathToolbox() :
  Toolbox(nullptr, &m_listController),
  m_selectableTableView(&m_listController, this, 0, 1, 0, 0, 0, 0, this, nullptr, false),
  m_listController(this, &m_selectableTableView),
  m_nodeModel(nullptr)
{
}

void MathToolbox::didBecomeFirstResponder() {
  app()->setFirstResponder(&m_listController);
}

bool MathToolbox::handleEvent(Ion::Events::Event event) {
  return handleEventForRow(event, selectedRow());
}

int MathToolbox::numberOfRows() {
  if (m_nodeModel == nullptr) {
    m_nodeModel = (ToolboxNode *)rootModel();
  }
  return m_nodeModel->numberOfChildren();
}

HighlightCell * MathToolbox::reusableCell(int index, int type) {
  assert(type < 2);
  assert(index >= 0);
  assert(index < k_maxNumberOfDisplayedRows);
  if (type == 0) {
    return &m_leafCells[index];
  }
  return &m_nodeCells[index];
}

int MathToolbox::reusableCellCount(int type) {
  return k_maxNumberOfDisplayedRows;
}

void MathToolbox::willDisplayCellForIndex(HighlightCell * cell, int index) {
  ToolboxNode * node = (ToolboxNode *)m_nodeModel->children(index);
  if (node->numberOfChildren() == 0) {
    MessageTableCellWithMessage * myCell = (MessageTableCellWithMessage *)cell;
    myCell->setMessage(node->label());
    myCell->setAccessoryMessage(node->text());
    myCell->setAccessoryTextColor(Palette::GreyDark);
    return;
  }
  MessageTableCell * myCell = (MessageTableCell *)cell;
  myCell->setMessage(node->label());
}

KDCoordinate MathToolbox::rowHeight(int j) {
  if (typeAtLocation(0, j) == 0) {
    return k_leafRowHeight;
  }
  return k_nodeRowHeight;
}

KDCoordinate MathToolbox::cumulatedHeightFromIndex(int j) {
  int result = 0;
  for (int k = 0; k < j; k++) {
    result += rowHeight(k);
  }
  return result;
}

int MathToolbox::indexFromCumulatedHeight(KDCoordinate offsetY) {
  int result = 0;
  int j = 0;
  while (result < offsetY && j < numberOfRows()) {
    result += rowHeight(j++);
  }
  return (result < offsetY || offsetY == 0) ? j : j - 1;
}

int MathToolbox::typeAtLocation(int i, int j) {
  Node * node = (Node *)m_nodeModel->children(j);
  if (node->numberOfChildren() == 0) {
    return 0;
  }
  return 1;
}

void MathToolbox::viewWillAppear() {
  Toolbox::viewWillAppear();
  m_nodeModel = (ToolboxNode *)rootModel();
  m_selectableTableView.reloadData();
  m_stack.resetStack();
  m_listController.setFirstSelectedRow(0);
}

void MathToolbox::viewDidDisappear() {
  Toolbox::viewDidDisappear();
  m_selectableTableView.deselectTable();
}

int MathToolbox::stackDepth() {
  return m_stack.depth();
}

TextField * MathToolbox::sender() {
  return (TextField *)Toolbox::sender();
}

bool MathToolbox::handleEventForRow(Ion::Events::Event event, int selectedRow) {
  int depth = m_stack.depth();
  if (event == Ion::Events::Back && depth == 0) {
    m_selectableTableView.deselectTable();
    app()->dismissModalViewController();
    return true;
  }
  if ((event == Ion::Events::Back || event == Ion::Events::Left) && depth > 0) {
    return returnToPreviousMenu();
  }
  ToolboxNode * selectedNode = (ToolboxNode *)m_nodeModel->children(selectedRow);
  if ((event == Ion::Events::OK || event == Ion::Events::EXE || event == Ion::Events::Right) && selectedNode->numberOfChildren() > 0) {
    return selectSubMenu(selectedNode);
  }
  if ((event == Ion::Events::OK || event == Ion::Events::EXE) && selectedNode->numberOfChildren() == 0) {
    return selectLeaf(selectedNode);
  }
  return false;
}

const ToolboxNode * MathToolbox::rootModel() {
  return &toolboxModel;
}

bool MathToolbox::selectLeaf(ToolboxNode * selectedNode){
  m_selectableTableView.deselectTable();
  ToolboxNode * node = selectedNode;
  const char * editedText = I18n::translate(node->insertedText());
  if (!sender()->isEditing()) {
    sender()->setEditing(true);
  }
  sender()->insertTextAtLocation(editedText, sender()->cursorLocation());
  int cursorDelta = 0;
  int editedTextLength = strlen(editedText);
  for (int i = 0; i < editedTextLength; i++) {
    if (editedText[i] == '(') {
      cursorDelta =  i + 1;
      break;
    }
  }
  sender()->setCursorLocation(sender()->cursorLocation()+cursorDelta);
  app()->dismissModalViewController();
  return true;
}

bool MathToolbox::returnToPreviousMenu() {
  m_selectableTableView.deselectTable();
  int depth = m_stack.depth();
  int index = 0;
  ToolboxNode * parentNode = (ToolboxNode *)rootModel();
  Stack::State * previousState = m_stack.stateAtIndex(index++);;
  while (depth-- > 1) {
    parentNode = (ToolboxNode *)parentNode->children(previousState->selectedRow());
    previousState = m_stack.stateAtIndex(index++);
  }
  m_selectableTableView.deselectTable();
  m_nodeModel = parentNode;
  m_listController.setFirstSelectedRow(previousState->selectedRow());
  KDPoint scroll = m_selectableTableView.contentOffset();
  m_selectableTableView.setContentOffset(KDPoint(scroll.x(), previousState->verticalScroll()));
  m_stack.pop();
  app()->setFirstResponder(&m_listController);
  return true;
}

bool MathToolbox::selectSubMenu(ToolboxNode * selectedNode) {
  m_stack.push(selectedRow(),  m_selectableTableView.contentOffset().y());
  m_selectableTableView.deselectTable();
  m_nodeModel = selectedNode;
  m_listController.setFirstSelectedRow(0);
  app()->setFirstResponder(&m_listController);
  return true;
}
