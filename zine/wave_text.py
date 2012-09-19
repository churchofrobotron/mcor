#!/usr/bin/env python

import sys

from collections import OrderedDict

int_to_english = (
    { 'cardinal': 'zero', 'ordinal': None, },
    { 'cardinal': 'one', 'ordinal': 'first', },
    { 'cardinal': 'two', 'ordinal': 'second', },
    { 'cardinal': 'three', 'ordinal': 'third', },
    { 'cardinal': 'four', 'ordinal': 'fourth', },
    { 'cardinal': 'five', 'ordinal': 'fifth', },
    { 'cardinal': 'six', 'ordinal': 'sixth', },
    { 'cardinal': 'seven', 'ordinal': 'seventh', },
    { 'cardinal': 'eight', 'ordinal': 'eighth', },
    { 'cardinal': 'nine', 'ordinal': 'ninth', },
    { 'cardinal': 'ten', 'ordinal': 'tenth', },
    { 'cardinal': 'eleven', 'ordinal': 'eleventh', },
    { 'cardinal': 'twelve', 'ordinal': 'twelfth', },
    { 'cardinal': 'thirteen', 'ordinal': 'thirteenth', },
    { 'cardinal': 'fourteen', 'ordinal': 'fourteenth', },
    { 'cardinal': 'fifteen', 'ordinal': 'fifteenth', },
    { 'cardinal': 'sixteen', 'ordinal': 'sixteenth', },
    { 'cardinal': 'seventeen', 'ordinal': 'seventeenth', },
    { 'cardinal': 'eighteen', 'ordinal': 'eighteenth', },
    { 'cardinal': 'nineteen', 'ordinal': 'nineteenth', },
    { 'cardinal': 'twenty', 'ordinal': 'twentieth', },
    { 'cardinal': 20, 'ordinal': 20, }, # 21
    { 'cardinal': 20, 'ordinal': 20, }, # 22
    { 'cardinal': 20, 'ordinal': 20, }, # 23
    { 'cardinal': 20, 'ordinal': 20, }, # 24
    { 'cardinal': 20, 'ordinal': 20, }, # 25
    { 'cardinal': 20, 'ordinal': 20, }, # 26
    { 'cardinal': 20, 'ordinal': 20, }, # 27
    { 'cardinal': 20, 'ordinal': 20, }, # 28
    { 'cardinal': 20, 'ordinal': 20, }, # 29
    { 'cardinal': 'thirty', 'ordinal': 'thirtieth', },
    { 'cardinal': 30, 'ordinal': 30, }, # 31
    { 'cardinal': 30, 'ordinal': 30, }, # 32
    { 'cardinal': 30, 'ordinal': 30, }, # 33
    { 'cardinal': 30, 'ordinal': 30, }, # 34
    { 'cardinal': 30, 'ordinal': 30, }, # 35
    { 'cardinal': 30, 'ordinal': 30, }, # 36
    { 'cardinal': 30, 'ordinal': 30, }, # 37
    { 'cardinal': 30, 'ordinal': 30, }, # 38
    { 'cardinal': 30, 'ordinal': 30, }, # 39
    { 'cardinal': 'fourty', 'ordinal': 'fourtieth', },
    { 'cardinal': 40, 'ordinal': 40, }, # 41
    { 'cardinal': 40, 'ordinal': 40, }, # 42
    { 'cardinal': 40, 'ordinal': 40, }, # 43
    { 'cardinal': 40, 'ordinal': 40, }, # 44
    { 'cardinal': 40, 'ordinal': 40, }, # 45
    { 'cardinal': 40, 'ordinal': 40, }, # 46
    { 'cardinal': 40, 'ordinal': 40, }, # 47
    { 'cardinal': 40, 'ordinal': 40, }, # 48
    { 'cardinal': 40, 'ordinal': 40, }, # 49
    { 'cardinal': 'fifty', 'ordinal': 'fiftieth', },
    { 'cardinal': 50, 'ordinal': 50, }, # 51
    { 'cardinal': 50, 'ordinal': 50, }, # 52
    { 'cardinal': 50, 'ordinal': 50, }, # 53
    { 'cardinal': 50, 'ordinal': 50, }, # 54
    { 'cardinal': 50, 'ordinal': 50, }, # 55
    { 'cardinal': 50, 'ordinal': 50, }, # 56
    { 'cardinal': 50, 'ordinal': 50, }, # 57
    { 'cardinal': 50, 'ordinal': 50, }, # 58
    { 'cardinal': 50, 'ordinal': 50, }, # 59
    { 'cardinal': 'sixty', 'ordinal': 'sixtieth', },
    { 'cardinal': 60, 'ordinal': 60, }, # 61
    { 'cardinal': 60, 'ordinal': 60, }, # 62
    { 'cardinal': 60, 'ordinal': 60, }, # 63
    { 'cardinal': 60, 'ordinal': 60, }, # 64
    { 'cardinal': 60, 'ordinal': 60, }, # 65
    { 'cardinal': 60, 'ordinal': 60, }, # 66
    { 'cardinal': 60, 'ordinal': 60, }, # 67
    { 'cardinal': 60, 'ordinal': 60, }, # 68
    { 'cardinal': 60, 'ordinal': 60, }, # 69
    { 'cardinal': 'seventy', 'ordinal': 'seventieth', },
    { 'cardinal': 70, 'ordinal': 70, }, # 71
    { 'cardinal': 70, 'ordinal': 70, }, # 72
    { 'cardinal': 70, 'ordinal': 70, }, # 73
    { 'cardinal': 70, 'ordinal': 70, }, # 74
    { 'cardinal': 70, 'ordinal': 70, }, # 75
    { 'cardinal': 70, 'ordinal': 70, }, # 76
    { 'cardinal': 70, 'ordinal': 70, }, # 77
    { 'cardinal': 70, 'ordinal': 70, }, # 78
    { 'cardinal': 70, 'ordinal': 70, }, # 79
    { 'cardinal': 'eighty', 'ordinal': 'eightieth', },
)



def to_english(n, number_type, formatter):
    result = int_to_english[n][number_type]
    if isinstance(result, int):
        remainder = n - result
        result = formatter(int_to_english[result]['cardinal'],
                           int_to_english[remainder][number_type])
    return result

def to_english_ordinal(n):
    return to_english(n, 'ordinal', lambda tens, ones: '%s-%s' % (tens, ones))

def to_english_cardinal(n):
    return to_english(n, 'cardinal', lambda tens, ones: '%s and %s' % (ones, tens))

def read_waves(f):
    header = f.readline().split()

    waves = []
    for line in f:
        row = map(int, line.split())
        wave = dict(zip(header, row))
        waves.append(wave)

    return waves

object_types = OrderedDict([
    ('Grunts',       { 'singular': 'Grunt',      }),
    ('Electrodes',   { 'singular': 'Electrode',  }),
    ('Mommies',      { 'singular': 'Mommy',      }),
    ('Daddies',      { 'singular': 'Daddy',      }),
    ('Mikeys',       { 'singular': 'Mikey',      }),
    ('Hulks',        { 'singular': 'Hulk',       }),
    ('Brains',       { 'singular': 'Brain',      }),
    ('Spheroids',    { 'singular': 'Spheroid',   }),
    ('Quarks',       { 'singular': 'Quark',      }),
])

def singular_or_plural(o):
    return o[0] if o[1] > 1 else object_types[o[0]]['singular']

def format_sentence(wave_number, sentence_plan, objects):
    object_count, sentence_format = sentence_plan
    
    substitutions = {
        'wave_number': to_english_ordinal(wave_number),
    }
    for n in range(object_count):
        o = objects.pop(0)
        substitutions['object%d_count' % n] = to_english_cardinal(o[1])
        substitutions['object%d_count_capitalize' % n] = substitutions['object%d_count' % n].capitalize()
        substitutions['object%d_name' % n] = singular_or_plural(o)
        substitutions['object%d_name_plural' % n] = o[0]
        
    return sentence_format % substitutions

def make_wave_text(wave):
    first_sentence_plan = (1, "On the %(wave_number)s wave there shall be %(object0_count)s %(object0_name)s.")
    middle_sentence_plan_even = (2, "%(object0_count_capitalize)s %(object0_name)s there shall be, and the number of %(object1_name_plural)s shall be %(object1_count)s.")
    middle_sentence_plan_odd = (1, "The number of %(object0_name_plural)s shall be %(object0_count)s.")
    last_sentence_plan = (3, "Of %(object0_name_plural)s, there shall be %(object0_count)s, the %(object1_name_plural)s shall number %(object1_count)s, and the %(object2_name_plural)s shall number %(object2_count)s.")
    
    objects = [(name, wave[name]) for name in object_types.keys() if name in wave and wave[name] > 0]

    wave_number = wave['Wave']
    
    sentences = []
    sentences.append(format_sentence(wave_number, first_sentence_plan, objects))
    
    while len(objects) > 3:
        sentence_plan = middle_sentence_plan_even if len(objects) & 1 else middle_sentence_plan_odd
        sentences.append(format_sentence(wave_number, sentence_plan, objects))
        
    sentences.append(format_sentence(wave_number, last_sentence_plan, objects))
    
    return ' '.join(sentences).upper()

f = open(sys.argv[1], 'r')
waves = read_waves(f)
for wave in waves:
    text = make_wave_text(wave)
    print(text)