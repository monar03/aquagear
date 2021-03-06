<?php
namespace aquagear\log;

/**
 * デバック用ログ関数.
 * 
 * @package aquagear
 * @subpackage log
 * 
 * @author motonari
 * @version $Id: DailyFileLogger.inc 535 2011-07-26 14:02:27Z motonari $
 */ 

class DailyFileLogger extends Logger implements ILogger {
	/**
	 * ログ出力レベルの設定.
	 * 
	 * @var object
	 */
	private $_level = 0;
		
	/**
	 * ログの出力をためる.
	 * 
	 * @var array
	 */
	private $_log_arr = array();

	/**
	 * 作成するディレクトリ.
	 * 
	 * @var string
	 */
	private $_dir_path = "";
	
	/**
	 * 作成するファイル名.
	 * 
	 * @var string
	 */
	private $_file_name = "";
	
	/**
	 * 標準出力の設定.
	 * 
	 * @var bool
	 */
	private $_stdout = false;
	
	/**
	 * 出力方法
	 * 
	 * @var string
	 */
	private $_output = false;
	
	/**
	 * 自インスタンスの取得.
	 * 
	 * @param mixed $property 制御値.
	 * @return object ログインスタンス.
	 */
	public static function getLogInstance($section='default') {
		throw new \aquagear\exception\LoggerException('can not execute '. __CLASS__ .'getLogInstance($property=null)', '500');
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/log/\aquagear\log\ILogger#initLogger()
	 */
	public function initLogger($conf_arr) {
		if(! empty($conf_arr)) {
			$this->_level  = $conf_arr['level'];
			$this->_stdout = $conf_arr['stdout'];
			$this->_dir_path = $conf_arr['dir_path'];
			$this->_file_name = $conf_arr['file_name'];
			$this->_output = $cont_arr['output'];
		}
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/log/\aquagear\log\ILogger#debug()
	 */
	public function debug($log_str) {
		$this->_printError("[DEBUG] : " . $log_str, 1);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/log/\aquagear\log\ILogger#info()
	 */
	public function info($log_str) {
		$this->_printError("[INFO] : " . $log_str, 2);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/log/\aquagear\log\ILogger#warning()
	 */
	public function warning($log_str) {
		$this->_printError("[WARN] : " . $log_str, 3);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/log/\aquagear\log\ILogger#error()
	 */
	public function error($log_str) {
		$this->_printError("[ERROR] : " . $log_str, 4);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/log/\aquagear\log\ILogger#fatal()
	 */
	public function fatal($log_str) {
		$this->_printError("[FATAL] : " . $log_str, 5);
	}
	
	/**
	 * エラーの出力.
	 * 
	 * @param $log_str
	 * @param $level
	 * @return unknown_type
	 */
	private function _printError($log_str, $level) {
		if($this->_level <= $level) {
			$head = date("Y M j G:i:s");
			$lines = explode("\n", $log_str);
			foreach($lines as $line) {
				if($this->_output === 'driven') {
					$date = date("Ymd");
					\aquagear\file\FileReaderWriter::write($this->_dir_path . '/' . $this->_file_name . '.' . $date, $line . "\n", FILE_APPEND | LOCK_EX, false);
				}
				else {			
					$this->_log_arr[] = '[' . $head . '] ' . $line;
				}
			}
		}
	}
	
	/**
	 * ログを出力.
	 * 
	 * @return void
	 */
	private function _finalize() {
		if(empty($this->_log_arr)) {
			return ;
		}
		
		if($this->_stdout) {
			print(implode('<br />', $this->_log_arr));
		}
		
		$date = date("Ymd");
		\aquagear\file\FileReaderWriter::write($this->_dir_path . '/' . $this->_file_name . '.' . $date, join("\n", $this->_log_arr) . "\n", FILE_APPEND | LOCK_EX, false);
		$this->_log_arr = array();
	}
	
	/**
	 * デストラクタ.
	 * 
	 * @return void
	 */
	public function __destruct(){
		$this->_finalize();
	}
}
?>