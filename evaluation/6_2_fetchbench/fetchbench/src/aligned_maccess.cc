#include "aligned_maccess.hh"

array<maccess_func_t, 256> maccess_noinline_funcs {
	maccess_1, maccess_2, maccess_3, maccess_4, maccess_5, maccess_6,
	maccess_7, maccess_8, maccess_9, maccess_10, maccess_11, maccess_12,
	maccess_13, maccess_14, maccess_15, maccess_16, maccess_17, maccess_18,
	maccess_19, maccess_20, maccess_21, maccess_22, maccess_23, maccess_24,
	maccess_25, maccess_26, maccess_27, maccess_28, maccess_29, maccess_30,
	maccess_31, maccess_32, maccess_33, maccess_34, maccess_35, maccess_36,
	maccess_37, maccess_38, maccess_39, maccess_40, maccess_41, maccess_42,
	maccess_43, maccess_44, maccess_45, maccess_46, maccess_47, maccess_48,
	maccess_49, maccess_50, maccess_51, maccess_52, maccess_53, maccess_54,
	maccess_55, maccess_56, maccess_57, maccess_58, maccess_59, maccess_60,
	maccess_61, maccess_62, maccess_63, maccess_64, maccess_65, maccess_66,
	maccess_67, maccess_68, maccess_69, maccess_70, maccess_71, maccess_72,
	maccess_73, maccess_74, maccess_75, maccess_76, maccess_77, maccess_78,
	maccess_79, maccess_80, maccess_81, maccess_82, maccess_83, maccess_84,
	maccess_85, maccess_86, maccess_87, maccess_88, maccess_89, maccess_90,
	maccess_91, maccess_92, maccess_93, maccess_94, maccess_95, maccess_96,
	maccess_97, maccess_98, maccess_99, maccess_100, maccess_101, maccess_102,
	maccess_103, maccess_104, maccess_105, maccess_106, maccess_107, maccess_108,
	maccess_109, maccess_110, maccess_111, maccess_112, maccess_113, maccess_114,
	maccess_115, maccess_116, maccess_117, maccess_118, maccess_119, maccess_120,
	maccess_121, maccess_122, maccess_123, maccess_124, maccess_125, maccess_126,
	maccess_127, maccess_128, maccess_129, maccess_130, maccess_131, maccess_132,
	maccess_133, maccess_134, maccess_135, maccess_136, maccess_137, maccess_138,
	maccess_139, maccess_140, maccess_141, maccess_142, maccess_143, maccess_144,
	maccess_145, maccess_146, maccess_147, maccess_148, maccess_149, maccess_150,
	maccess_151, maccess_152, maccess_153, maccess_154, maccess_155, maccess_156,
	maccess_157, maccess_158, maccess_159, maccess_160, maccess_161, maccess_162,
	maccess_163, maccess_164, maccess_165, maccess_166, maccess_167, maccess_168,
	maccess_169, maccess_170, maccess_171, maccess_172, maccess_173, maccess_174,
	maccess_175, maccess_176, maccess_177, maccess_178, maccess_179, maccess_180,
	maccess_181, maccess_182, maccess_183, maccess_184, maccess_185, maccess_186,
	maccess_187, maccess_188, maccess_189, maccess_190, maccess_191, maccess_192,
	maccess_193, maccess_194, maccess_195, maccess_196, maccess_197, maccess_198,
	maccess_199, maccess_200, maccess_201, maccess_202, maccess_203, maccess_204,
	maccess_205, maccess_206, maccess_207, maccess_208, maccess_209, maccess_210,
	maccess_211, maccess_212, maccess_213, maccess_214, maccess_215, maccess_216,
	maccess_217, maccess_218, maccess_219, maccess_220, maccess_221, maccess_222,
	maccess_223, maccess_224, maccess_225, maccess_226, maccess_227, maccess_228,
	maccess_229, maccess_230, maccess_231, maccess_232, maccess_233, maccess_234,
	maccess_235, maccess_236, maccess_237, maccess_238, maccess_239, maccess_240,
	maccess_241, maccess_242, maccess_243, maccess_244, maccess_245, maccess_246,
	maccess_247, maccess_248, maccess_249, maccess_250, maccess_251, maccess_252,
	maccess_253, maccess_254, maccess_255, maccess_256
};

/**
 * Returns a pair of two function pointers to two non-inlining maccess
 * functions that are aligned at a 2^(colliding_bits) boundary.
 *
 * @param[in]  colliding_bits  The number of colliding bits
 *
 * @return     std::pair of function pointers to two maccess functions.
 */
pair<maccess_func_t,maccess_func_t> get_maccess_functions(size_t colliding_bits) {
	assert(colliding_bits >= 5 && colliding_bits <= 24);
	pair<maccess_func_t, maccess_func_t> funcs;
	switch(colliding_bits) {
		case 5:
			funcs = {maccess_5_1, maccess_5_2};
			break;
		case 6:
			funcs = {maccess_6_1, maccess_6_2};
			break;
		case 7:
			funcs = {maccess_7_1, maccess_7_2};
			break;
		case 8:
			funcs = {maccess_8_1, maccess_8_2};
			break;
		case 9:
			funcs = {maccess_9_1, maccess_9_2};
			break;
		case 10:
			funcs = {maccess_10_1, maccess_10_2};
			break;
		case 11:
			funcs = {maccess_11_1, maccess_11_2};
			break;
		case 12:
			funcs = {maccess_12_1, maccess_12_2};
			break;
		case 13:
			funcs = {maccess_13_1, maccess_13_2};
			break;
		case 14:
			funcs = {maccess_14_1, maccess_14_2};
			break;
		case 15:
			funcs = {maccess_15_1, maccess_15_2};
			break;
		case 16:
			funcs = {maccess_16_1, maccess_16_2};
			break;
		case 17:
			funcs = {maccess_17_1, maccess_17_2};
			break;
		case 18:
			funcs = {maccess_18_1, maccess_18_2};
			break;
		case 19:
			funcs = {maccess_19_1, maccess_19_2};
			break;
		case 20:
			funcs = {maccess_20_1, maccess_20_2};
			break;
		case 21:
			funcs = {maccess_21_1, maccess_21_2};
			break;
		case 22:
			funcs = {maccess_22_1, maccess_22_2};
			break;
		case 23:
			funcs = {maccess_23_1, maccess_23_2};
			break;
		case 24:
			funcs = {maccess_24_1, maccess_24_2};
			break;
		default:
			assert(false);
			break;
	}

	// make sure the compiler did what we intended

	// ensure that the lower (colliding_bits) bits of both functions are
	// actually equal
	assert(
		(((uintptr_t)funcs.first) & ((1ULL << colliding_bits)-1)) ==
		(((uintptr_t)funcs.second) & ((1ULL << colliding_bits)-1))
	);
	// ensure that the upper bits of both functions are actually different
	assert(
		(((uintptr_t)funcs.first) & ~((1ULL << colliding_bits)-1)) !=
		(((uintptr_t)funcs.second) & ~((1ULL << colliding_bits)-1))
	);
	// ensure that especially the (colliding_bits+1)'th bit is different
	assert(
		((((uintptr_t)funcs.first) >> colliding_bits) & 0b1) !=
		((((uintptr_t)funcs.second) >> colliding_bits) & 0b1)
	);

	return funcs;
}

maccess_func_t get_random_maccess_noinline_func() {
	return maccess_noinline_funcs[random_uint32(0, maccess_noinline_funcs.size() - 1)];
}

__attribute__((aligned(1 << 5))) __attribute__ ((noinline)) void maccess_5_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 5))) __attribute__ ((noinline)) void maccess_5_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 6))) __attribute__ ((noinline)) void maccess_6_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 6))) __attribute__ ((noinline)) void maccess_6_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 7))) __attribute__ ((noinline)) void maccess_7_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 7))) __attribute__ ((noinline)) void maccess_7_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 8))) __attribute__ ((noinline)) void maccess_8_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 8))) __attribute__ ((noinline)) void maccess_8_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 9))) __attribute__ ((noinline)) void maccess_9_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 9))) __attribute__ ((noinline)) void maccess_9_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 10))) __attribute__ ((noinline)) void maccess_10_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 10))) __attribute__ ((noinline)) void maccess_10_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 11))) __attribute__ ((noinline)) void maccess_11_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 11))) __attribute__ ((noinline)) void maccess_11_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 12))) __attribute__ ((noinline)) void maccess_12_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 12))) __attribute__ ((noinline)) void maccess_12_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 13))) __attribute__ ((noinline)) void maccess_13_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 13))) __attribute__ ((noinline)) void maccess_13_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 14))) __attribute__ ((noinline)) void maccess_14_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 14))) __attribute__ ((noinline)) void maccess_14_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 15))) __attribute__ ((noinline)) void maccess_15_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 15))) __attribute__ ((noinline)) void maccess_15_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 16))) __attribute__ ((noinline)) void maccess_16_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 16))) __attribute__ ((noinline)) void maccess_16_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 17))) __attribute__ ((noinline)) void maccess_17_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 17))) __attribute__ ((noinline)) void maccess_17_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 18))) __attribute__ ((noinline)) void maccess_18_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 18))) __attribute__ ((noinline)) void maccess_18_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 19))) __attribute__ ((noinline)) void maccess_19_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 19))) __attribute__ ((noinline)) void maccess_19_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 20))) __attribute__ ((noinline)) void maccess_20_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 20))) __attribute__ ((noinline)) void maccess_20_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 21))) __attribute__ ((noinline)) void maccess_21_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 21))) __attribute__ ((noinline)) void maccess_21_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 22))) __attribute__ ((noinline)) void maccess_22_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 22))) __attribute__ ((noinline)) void maccess_22_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 23))) __attribute__ ((noinline)) void maccess_23_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 23))) __attribute__ ((noinline)) void maccess_23_2(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 24))) __attribute__ ((noinline)) void maccess_24_1(void* p) {
	maccess(p);
}

__attribute__((aligned(1 << 24))) __attribute__ ((noinline)) void maccess_24_2(void* p) {
	maccess(p);
}

// non inline maccess for creating multiple entries in SMS prefetcher
__attribute__ ((noinline)) void maccess_1(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_2(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_3(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_4(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_5(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_6(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_7(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_8(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_9(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_10(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_11(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_12(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_13(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_14(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_15(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_16(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_17(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_18(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_19(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_20(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_21(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_22(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_23(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_24(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_25(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_26(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_27(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_28(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_29(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_30(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_31(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_32(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_33(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_34(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_35(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_36(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_37(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_38(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_39(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_40(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_41(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_42(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_43(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_44(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_45(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_46(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_47(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_48(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_49(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_50(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_51(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_52(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_53(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_54(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_55(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_56(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_57(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_58(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_59(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_60(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_61(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_62(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_63(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_64(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_65(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_66(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_67(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_68(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_69(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_70(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_71(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_72(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_73(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_74(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_75(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_76(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_77(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_78(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_79(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_80(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_81(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_82(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_83(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_84(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_85(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_86(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_87(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_88(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_89(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_90(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_91(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_92(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_93(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_94(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_95(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_96(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_97(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_98(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_99(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_100(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_101(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_102(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_103(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_104(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_105(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_106(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_107(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_108(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_109(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_110(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_111(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_112(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_113(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_114(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_115(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_116(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_117(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_118(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_119(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_120(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_121(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_122(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_123(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_124(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_125(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_126(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_127(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_128(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_129(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_130(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_131(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_132(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_133(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_134(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_135(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_136(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_137(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_138(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_139(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_140(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_141(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_142(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_143(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_144(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_145(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_146(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_147(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_148(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_149(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_150(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_151(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_152(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_153(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_154(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_155(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_156(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_157(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_158(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_159(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_160(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_161(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_162(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_163(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_164(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_165(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_166(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_167(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_168(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_169(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_170(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_171(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_172(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_173(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_174(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_175(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_176(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_177(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_178(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_179(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_180(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_181(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_182(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_183(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_184(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_185(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_186(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_187(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_188(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_189(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_190(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_191(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_192(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_193(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_194(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_195(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_196(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_197(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_198(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_199(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_200(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_201(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_202(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_203(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_204(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_205(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_206(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_207(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_208(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_209(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_210(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_211(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_212(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_213(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_214(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_215(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_216(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_217(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_218(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_219(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_220(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_221(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_222(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_223(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_224(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_225(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_226(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_227(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_228(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_229(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_230(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_231(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_232(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_233(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_234(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_235(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_236(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_237(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_238(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_239(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_240(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_241(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_242(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_243(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_244(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_245(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_246(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_247(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_248(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_249(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_250(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_251(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_252(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_253(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_254(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_255(void* p) { maccess(p); }
__attribute__ ((noinline)) void maccess_256(void* p) { maccess(p); }
