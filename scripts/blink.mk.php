<?php
$input = file_get_contents(dirname(__FILE__) . '/blink.mk.txt');
preg_match_all('/\/renderer(\/.*)/', $input, $matches);
$objects = array();
foreach ($matches[1] as $v) {
    $file = $v;

    preg_match('/.+\/(.*?)\./', $file, $parts);
    $obj = $parts[1] . '.o';

    $objects[]= compact('obj', 'file');
}
?>BlinkSrc = $(BkRoot)src/chromium/third_party/blink/renderer
BlinkFlags = -I$(BkRoot)src/blink -I$(BkRoot)src/stub/icu -DBLINKIT_CRAWLER_ONLY $(CrFlags)
BlinkObjects =<?php foreach ($objects as $v) { echo ' ' . $v['obj']; } ?>


<?php foreach ($objects as $v) : ?>
<?php echo $v['obj']; ?>: $(BlinkSrc)<?php echo $v['file'] . "\n"; ?>
	$(CXX) -c $(CXXFLAGS) $(BlinkFlags) $< -o $@
<?php endforeach; ?>
