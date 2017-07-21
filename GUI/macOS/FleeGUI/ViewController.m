//
//  ViewController.m
//  FleeGUI
//
//  Created by Unit.2B on 2017/7/21.
//

#import "ViewController.h"

@import NetworkExtension;

@interface ViewController ()

@property (retain) NETunnelProviderManager *tunnelManager;

@property (weak) IBOutlet NSTextField *urlTextField;
@property (weak) IBOutlet NSButton *actionButton;
@property (unsafe_unretained) IBOutlet NSTextView *consoleTextView;

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];

  [NETunnelProviderManager
      loadAllFromPreferencesWithCompletionHandler:^(
          NSArray<NETunnelProviderManager *> *_Nullable managers,
          NSError *_Nullable error) {
        if ([managers count] > 0) {
          self.tunnelManager = managers.firstObject;
          [self appendConsole:@"Tunnel Manager Found"];
        } else {
          self.tunnelManager = [[NETunnelProviderManager alloc] init];
          [self appendConsole:@"Tunnel Manager Created"];
        }
      }];
}

- (IBAction)onActionButtonClicked:(id)sender {
  NSURL *url = [[NSURL alloc] initWithString:self.urlTextField.stringValue];
  if (![url.scheme isEqualToString:@"flee"]) {
    [self appendConsole:@"URL scheme is not 'flee'"];
    return;
  }
}

- (void)appendConsole:(NSString *)line {
  NSDate *now = [NSDate new];
  NSDateFormatter *df = [[NSDateFormatter alloc] init];
  [df setDateFormat:@"HH:mm:ss"];
  NSString *content = self.consoleTextView.string;
  if (content == nil) {
    content = @"";
  }
  NSMutableString *mContent = [NSMutableString stringWithString:content];
  if (mContent.length != 0) {
    [mContent appendString:@"\n"];
  }
  [mContent appendFormat:@"[%@] %@", [df stringFromDate:now], line];
  self.consoleTextView.string = [mContent copy];
  [self.consoleTextView scrollToEndOfDocument:nil];
}

@end
