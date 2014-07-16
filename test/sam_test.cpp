#include <boost/test/unit_test.hpp>

#include "sam.h"
#include "sam_reader.h"
#include "sam_builder.h"
#include "is_missing.h"

#include <vector>

using namespace std;
using namespace gamgee;


BOOST_AUTO_TEST_CASE( sam_body_simple_members_by_reference ) {
  const auto chr = 5u;
  const auto aln = 1000u;
  const auto expected_cigar = "76M";
  const auto expected_cigar_size = 1;
  const auto expected_cigar_element_length = 76;
  const auto expected_cigar_element_operator = CigarOperator::M;
  const auto expected_bases = "ACCCTAACCCTAACCCTAACCCTAACCATAACCCTAAGACTAACCCTAAACCTAACCCTCATAATCGAAATACAAC";
  const vector<uint8_t> expected_quals = {33, 33, 33, 33, 34, 31, 34, 30, 32, 32, 33, 34, 33, 33, 27, 21, 18, 29, 28, 33, 31, 29, 10, 33, 24, 12, 24, 10, 8, 17, 33, 23, 11, 10, 31, 18, 17, 22, 33, 20, 32, 29, 24, 15, 7, 7, 29, 12, 10, 6, 6, 18, 30, 7, 14, 6, 6, 6, 32, 8, 7, 6, 6, 16, 24, 7, 6, 22, 13, 11, 9, 9, 4, 8, 18, 25};

  for (auto& record : SingleSamReader {"testdata/test_simple.bam"}) {  // should be replaced by mock sam_body
    record.set_chromosome(chr);
    BOOST_CHECK_EQUAL(record.chromosome(), chr);
    record.set_alignment_start(aln);
    BOOST_CHECK_EQUAL(record.alignment_start(), aln);
    record.set_mate_chromosome(chr);
    BOOST_CHECK_EQUAL(record.mate_chromosome(), chr);
    record.set_mate_alignment_start(aln);
    BOOST_CHECK_EQUAL(record.mate_alignment_start(), aln);

    const auto actual_cigar = record.cigar();
    BOOST_CHECK_EQUAL(actual_cigar.to_string(), expected_cigar);
    BOOST_CHECK_EQUAL(actual_cigar.size(), expected_cigar_size);
    BOOST_CHECK_EQUAL(static_cast<int>(Cigar::cigar_op(actual_cigar[0])), static_cast<int>(expected_cigar_element_operator));
    BOOST_CHECK_EQUAL(Cigar::cigar_oplen(actual_cigar[0]), expected_cigar_element_length);
    BOOST_CHECK_EQUAL(record.bases().to_string(), expected_bases);
    const auto actual_quals = record.base_quals();
    BOOST_CHECK_EQUAL(actual_quals.size(), expected_quals.size());
    for ( auto i = 0u; i < actual_quals.size(); ++i ) {
      BOOST_CHECK_EQUAL(actual_quals[i], expected_quals[i]);
    }

    break;
  }
}

BOOST_AUTO_TEST_CASE( sam_body_simple_members_by_copy ) {
  const auto chr = 5u;
  const auto aln = 1000u;
  const auto expected_cigar = "76M";
  const auto expected_cigar_size = 1;
  const auto expected_cigar_element_length = 76;
  const auto expected_cigar_element_operator = CigarOperator::M;
  const auto expected_bases = "ACCCTAACCCTAACCCTAACCCTAACCATAACCCTAAGACTAACCCTAAACCTAACCCTCATAATCGAAATACAAC";
  const vector<uint8_t> expected_quals = {33, 33, 33, 33, 34, 31, 34, 30, 32, 32, 33, 34, 33, 33, 27, 21, 18, 29, 28, 33, 31, 29, 10, 33, 24, 12, 24, 10, 8, 17, 33, 23, 11, 10, 31, 18, 17, 22, 33, 20, 32, 29, 24, 15, 7, 7, 29, 12, 10, 6, 6, 18, 30, 7, 14, 6, 6, 6, 32, 8, 7, 6, 6, 16, 24, 7, 6, 22, 13, 11, 9, 9, 4, 8, 18, 25};

  for (auto record : SingleSamReader {"testdata/test_simple.bam"}) {  // should be replaced by mock sam_body
    record.set_chromosome(chr);
    BOOST_CHECK_EQUAL(record.chromosome(), chr);
    record.set_alignment_start(aln);
    BOOST_CHECK_EQUAL(record.alignment_start(), aln);
    record.set_mate_chromosome(chr);
    BOOST_CHECK_EQUAL(record.mate_chromosome(), chr);
    record.set_mate_alignment_start(aln);
    BOOST_CHECK_EQUAL(record.mate_alignment_start(), aln);

    // TODO: more comprehensive tests for variable-length data fields once setters are in place
    const auto actual_cigar = record.cigar();
    BOOST_CHECK_EQUAL(actual_cigar.to_string(), expected_cigar);
    BOOST_CHECK_EQUAL(actual_cigar.size(), expected_cigar_size);
    BOOST_CHECK_EQUAL(static_cast<int>(Cigar::cigar_op(actual_cigar[0])), static_cast<int>(expected_cigar_element_operator));
    BOOST_CHECK_EQUAL(Cigar::cigar_oplen(actual_cigar[0]), expected_cigar_element_length);

    BOOST_CHECK_EQUAL(record.bases().to_string(), expected_bases);

    const auto actual_quals = record.base_quals();
    BOOST_CHECK_EQUAL(actual_quals.size(), expected_quals.size());
    for ( auto i = 0u; i < actual_quals.size(); ++i ) {
      BOOST_CHECK_EQUAL(actual_quals[i], expected_quals[i]);
    }

    break;
  }
}

BOOST_AUTO_TEST_CASE( sam_body_flags ) 
{
  for (auto record : SingleSamReader {"testdata/test_simple.bam"}) {  // should be replaced by mock sam_body
    record.set_paired();
    BOOST_CHECK(record.paired());
    record.set_not_paired();
    BOOST_CHECK(!record.paired());
    record.set_unmapped();
    BOOST_CHECK(record.unmapped());
    record.set_not_unmapped();
    BOOST_CHECK(!record.unmapped());
    record.set_next_unmapped();
    BOOST_CHECK(record.next_unmapped());
    record.set_not_next_unmapped();
    BOOST_CHECK(!record.next_unmapped());
    record.set_reverse();
    BOOST_CHECK(record.reverse());
    record.set_not_reverse();
    BOOST_CHECK(!record.reverse());
    record.set_next_reverse();
    BOOST_CHECK(record.next_reverse());
    record.set_not_next_reverse();
    BOOST_CHECK(!record.next_reverse());
    record.set_first();
    BOOST_CHECK(record.first());
    record.set_not_first();
    BOOST_CHECK(!record.first());
    record.set_last();
    BOOST_CHECK(record.last());
    record.set_not_last();
    BOOST_CHECK(!record.last());
    record.set_secondary();
    BOOST_CHECK(record.secondary());
    record.set_not_secondary();
    BOOST_CHECK(!record.secondary());
    record.set_fail();
    BOOST_CHECK(record.fail());
    record.set_not_fail();
    BOOST_CHECK(!record.fail());
    record.set_duplicate();
    BOOST_CHECK(record.duplicate());
    record.set_not_duplicate();
    BOOST_CHECK(!record.duplicate());
    record.set_supplementary();
    BOOST_CHECK(record.supplementary());
    record.set_not_supplementary();
    BOOST_CHECK(!record.supplementary());
    break;  // only do this to one read
  }
}

BOOST_AUTO_TEST_CASE( sam_in_place_base_quals_modification ) {
  auto read = *(SingleSamReader{"testdata/test_simple.bam"}.begin());
  auto read_quals = read.base_quals();
  auto expected_quals = vector<uint8_t>{4, 33, 50, 42, 34, 31};

  read_quals[0] = 4;
  read_quals[2] = 50;
  read_quals[3] = 42;

  for ( auto i = 0u; i < expected_quals.size(); ++i ) {
    BOOST_CHECK_EQUAL(read_quals[i], expected_quals[i]);
  }
}

BOOST_AUTO_TEST_CASE( sam_in_place_bases_modification ) {
  auto read = *(SingleSamReader{"testdata/test_simple.bam"}.begin());
  auto read_bases = read.bases();
  auto expected_bases = vector<Base>{ Base::N, Base::C, Base::G, Base::C, Base::T, Base::C, Base::A, Base::N, Base::N, Base::T, Base::T, Base::A, Base::A };

  // These test modification of the lower 4 bits only, upper 4 bits only, and both upper and lower bits
  read_bases.set_base(0, Base::N);
  read_bases.set_base(2, Base::G);
  read_bases.set_base(5, Base::C);
  read_bases.set_base(7, Base::N);
  read_bases.set_base(8, Base::N);
  read_bases.set_base(9, Base::T);

  for ( auto i = 0u; i < expected_bases.size(); ++i ) {
    BOOST_CHECK_EQUAL(int(read_bases[i]), int(expected_bases[i]));
  }
}

BOOST_AUTO_TEST_CASE( sam_in_place_cigar_modification ) {
  auto read = *(SingleSamReader{"testdata/test_simple.bam"}.begin());
  auto read_cigar = read.cigar();

  read_cigar[0] = Cigar::make_cigar_element(30, CigarOperator::I);

  BOOST_CHECK_EQUAL(read_cigar[0], Cigar::make_cigar_element(30, CigarOperator::I));
}

BOOST_AUTO_TEST_CASE( sam_read_tags ) {
  const auto read1 = *(SingleSamReader{"testdata/test_simple.bam"}.begin());
  const auto read2 = *(SingleSamReader{"testdata/test_paired.bam"}.begin());

  const auto read1_pg_tag = read1.string_tag("PG");
  BOOST_CHECK_EQUAL(read1_pg_tag.name(), "PG");
  BOOST_CHECK_EQUAL(read1_pg_tag.value(), "0");
  BOOST_CHECK_EQUAL(read1_pg_tag.is_present(), true);

  const auto read1_rg_tag = read1.string_tag("RG");
  BOOST_CHECK_EQUAL(read1_rg_tag.name(), "RG");
  BOOST_CHECK_EQUAL(read1_rg_tag.value(), "exampleBAM.bam");
  BOOST_CHECK_EQUAL(read1_rg_tag.is_present(), true);

  const auto read1_sm_tag = read1.string_tag("SM");
  BOOST_CHECK_EQUAL(read1_sm_tag.name(), "SM");
  BOOST_CHECK_EQUAL(read1_sm_tag.value(), "exampleBAM.bam");
  BOOST_CHECK_EQUAL(read1_sm_tag.is_present(), true);

  const auto read1_nonexistent_tag = read1.integer_tag("DR");
  BOOST_CHECK_EQUAL(read1_nonexistent_tag.is_present(), false);

  const auto read2_nm_tag = read2.integer_tag("NM");
  BOOST_CHECK_EQUAL(read2_nm_tag.name(), "NM");
  BOOST_CHECK_EQUAL(read2_nm_tag.value(), 0);
  BOOST_CHECK_EQUAL(read2_nm_tag.is_present(), true);

  const auto read2_md_tag = read2.string_tag("MD");
  BOOST_CHECK_EQUAL(read2_md_tag.name(), "MD");
  BOOST_CHECK_EQUAL(read2_md_tag.value(), "76");
  BOOST_CHECK_EQUAL(read2_md_tag.is_present(), true);

  const auto read2_as_tag = read2.integer_tag("AS");
  BOOST_CHECK_EQUAL(read2_as_tag.name(), "AS");
  BOOST_CHECK_EQUAL(read2_as_tag.value(), 76);
  BOOST_CHECK_EQUAL(read2_as_tag.is_present(), true);

  const auto read2_xs_tag = read2.integer_tag("XS");
  BOOST_CHECK_EQUAL(read2_xs_tag.name(), "XS");
  BOOST_CHECK_EQUAL(read2_xs_tag.value(), 0);
  BOOST_CHECK_EQUAL(read2_xs_tag.is_present(), true);

  // check integer, char and float tags
  const auto read1_za_tag = read1.double_tag("ZA");
  BOOST_CHECK_EQUAL(read1_za_tag.name(), "ZA");
  BOOST_CHECK_CLOSE(read1_za_tag.value(), 2.3, 0.001);
  BOOST_CHECK_EQUAL(read1_za_tag.is_present(), true);
  const auto read1_zb_tag = read1.integer_tag("ZB");
  BOOST_CHECK_EQUAL(read1_zb_tag.name(), "ZB");
  BOOST_CHECK_EQUAL(read1_zb_tag.value(), 23);
  BOOST_CHECK_EQUAL(read1_zb_tag.is_present(), true);
  const auto read1_zc_tag = read1.char_tag("ZC");
  BOOST_CHECK_EQUAL(read1_zc_tag.name(), "ZC");
  BOOST_CHECK_EQUAL(read1_zc_tag.value(), 't');
  BOOST_CHECK_EQUAL(read1_zc_tag.is_present(), true);

  // check is_missing functionality on missing tags
  const auto read2_nonexistent_string_tag = read2.string_tag("PP");
  BOOST_CHECK_EQUAL(read2_nonexistent_string_tag.is_present(), false);
  BOOST_CHECK(is_missing(read2_nonexistent_string_tag));
  const auto read2_nonexistent_integer_tag = read2.integer_tag("PP");
  BOOST_CHECK_EQUAL(read2_nonexistent_integer_tag.is_present(), false);
  BOOST_CHECK(is_missing(read2_nonexistent_integer_tag));
  const auto read2_nonexistent_double_tag = read2.double_tag("PP");
  BOOST_CHECK_EQUAL(read2_nonexistent_double_tag.is_present(), false);
  BOOST_CHECK(is_missing(read2_nonexistent_double_tag));
  const auto read2_nonexistent_char_tag = read2.char_tag("PP");
  BOOST_CHECK_EQUAL(read2_nonexistent_char_tag.is_present(), false);
  BOOST_CHECK(is_missing(read2_nonexistent_string_tag));

  // miising value due to type mismatches
  const auto not_a_char_tag = read1.char_tag("ZB");     // ZB is an integer tag
  BOOST_CHECK(is_missing(not_a_char_tag));              // this should yield "not a char" which is equal to a missing value
  const auto not_a_string_tag = read1.string_tag("ZB"); // ZB is an integer tag
  BOOST_CHECK(is_missing(not_a_string_tag));            // this should yield "not a char" which is equal to a missing value
}

BOOST_AUTO_TEST_CASE( sam_copy_constructor ) {
  const auto read1 = *(SingleSamReader{"testdata/test_simple.bam"}.begin());
  auto read2 = read1; // copy read1
  read2.set_alignment_start(5000);
  BOOST_CHECK(read1.alignment_start() != read2.alignment_start());
  read2 = read1; // copy assignment test 
  BOOST_CHECK_EQUAL(read1.alignment_start(), read2.alignment_start());
  read2.set_alignment_start(1);
  read2 = read2; // check self assignment
  BOOST_CHECK_EQUAL(read2.alignment_start(), 1);
  auto read3 = read1; // check that variable length data field modifications don't affect the original record
  read3.base_quals()[0] = 90;
  BOOST_CHECK(read1.base_quals()[0] != read3.base_quals()[0]);
}

void check_read_alignment_starts_and_stops(const Sam& read, const uint32_t astart, const uint32_t astop, const uint32_t ustart, const uint32_t ustop) {
  BOOST_CHECK_EQUAL(read.alignment_start(), astart);
  BOOST_CHECK_EQUAL(read.alignment_stop(), astop);
  BOOST_CHECK_EQUAL(read.unclipped_start(), ustart);
  BOOST_CHECK_EQUAL(read.unclipped_stop(), ustop);
}

BOOST_AUTO_TEST_CASE( sam_unclipped_start_and_stop ) {
  const auto header = SingleSamReader{"testdata/test_simple.bam"}.header();
  auto builder = SamBuilder{header, false};
  const auto sam1 = builder.set_chromosome(0).set_alignment_start(100).set_cigar("20M").build();  // these are not complete reads, they only work for testing purposes!
  const auto sam2 = builder.set_chromosome(0).set_alignment_start(100).set_cigar("5S15M").build(); 
  const auto sam3 = builder.set_chromosome(0).set_alignment_start(100).set_cigar("15M5S").build(); 
  const auto sam4 = builder.set_chromosome(0).set_alignment_start(100).set_cigar("5S10M5S").build(); 
  check_read_alignment_starts_and_stops(sam1, 100, 120, 100, 120);
  check_read_alignment_starts_and_stops(sam2, 100, 115, 95, 115);
  check_read_alignment_starts_and_stops(sam3, 100, 115, 100, 120);
  check_read_alignment_starts_and_stops(sam4, 100, 110, 95, 115);
}